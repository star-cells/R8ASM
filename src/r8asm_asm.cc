#include "r8asm/r8asm_asm.hh"

#include <string>
#include <variant>
#include <vector>

static r8asm_data
read_opr(R8Operand arg, r8asm_data default_opr = 0)
{
    if (const std::string *operand = std::get_if<std::string>(&arg)) {
	if ((*operand) == "$")
	    return dataptr;
	return (datas[*operand]);
    } else if (const r8asm_data *operand = std::get_if<r8asm_data>(&arg))
	return (*operand);
    return (default_opr);
}

static void expand_xor(std::vector<rot8_bytecode> &tape, R8Operand arg)
{
    r8asm_data opr = read_opr(arg);

    for (unit_size_type i = 0; i < UNIT_SIZE; i++) {
	if ((opr & 1) == 1)
	    tape.push_back(rot8_bytecode::FLB);
	tape.push_back(rot8_bytecode::ROR);
	opr >>= 1;
    }
}

static void expand_loop(std::vector<rot8_bytecode> &tape, R8Operand arg)
{
    r8asm_data opr = read_opr(arg, UNIT_MAX);

    tape.push_back(rot8_bytecode::STP);
    expand_xor(tape, opr);
    tape.push_back(rot8_bytecode::BIZ);
    tape.push_back(rot8_bytecode::BTP); // >(xor)[<
}

static void expand_endloop(std::vector<rot8_bytecode> &tape, R8Operand arg)
{
    std::vector<rot8_bytecode> tmp = { rot8_bytecode::STP,
	rot8_bytecode::FLB,
	rot8_bytecode::ROR,
	rot8_bytecode::RNZ,
	rot8_bytecode::BTP }; // >-+]<
    tape.insert(tape.end(), tmp.begin(), tmp.end());
}

static void expand_tp(std::vector<rot8_bytecode> &tape, R8Operand arg)
{
    r8asm_data opr = read_opr(arg);

    unit_offset_type offset = (unit_offset_type)opr - (unit_offset_type)dataptr;
    bool stepforward = (offset > 0);
    offset = abs(offset);
    for (unit_offset_type i = 0; i < offset; i++) {
	tape.push_back(stepforward ? rot8_bytecode::STP : rot8_bytecode::BTP);
    }
    dataptr = opr;
}

static void expand_anchor(std::vector<rot8_bytecode> &tape, R8Operand arg)
{
    if (const std::string *name = std::get_if<std::string>(&arg))
	datas.insert({ *name, dataptr });
    else if (const r8asm_data *name = std::get_if<r8asm_data>(&arg))
	datas.insert({ std::to_string(*name), dataptr });
}

std::vector<rot8_bytecode>
r8asm_tape_out(std::span<const R8Instruction> ops)
{
    std::vector<rot8_bytecode> tape;
    for (const auto it : ops) {
	if (const rot8_bytecode *bc = std::get_if<rot8_bytecode>(&it.op)) {
	    r8asm_data repeat = read_opr(it.arg);
	    for (r8asm_data i = 0; i < repeat; i++) {
		tape.push_back(*bc);
		if (*bc == rot8_bytecode::STP)
		    dataptr++;
		else if (*bc == rot8_bytecode::BTP)
		    dataptr--;
	    }
	} else if (const r8asm_macro *macro = std::get_if<r8asm_macro>(&it.op))
	    switch (*macro) {
	    case r8asm_macro::LOOP:
		expand_loop(tape, it.arg);
		break;
	    case r8asm_macro::ENDLOOP:
		expand_endloop(tape, it.arg);
		break;
	    case r8asm_macro::TP:;
		expand_tp(tape, it.arg);
		break;
	    case r8asm_macro::XOR:
		expand_xor(tape, it.arg);
		break;
	    case r8asm_macro::ANCHOR:
		expand_anchor(tape, it.arg);
		break;
	    default:;
	    }
    }
    return (tape);
}
