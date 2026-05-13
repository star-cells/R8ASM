#include "r8asm/r8asm_asm.hh"
#include "r8asm/r8asm_core.hh"

#include <string>
#include <variant>
#include <vector>

r8asm_data read_opr(R8Operand arg,
		    r8asm_data default_opr = 0) // Attention,default_opr is 0
{
    if (const std::string *operand = std::get_if<std::string>(&arg)) {
	if ((*operand) == "$")
	    return dataptr;
	return (datas[*operand]);
    } else if (const r8asm_data *operand = std::get_if<r8asm_data>(&arg))
	return (*operand);
    return (default_opr);
}

void expand_xor(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    r8asm_data opr = read_opr(arg);

    for (unit_size_type i = 0; i < UNIT_SIZE; i++) {
	if ((opr & 1) == 1)
	    tape.push_back(rot8_bytecode::FLB);
	tape.push_back(rot8_bytecode::ROR);
	opr >>= 1;
    }
}

void expand_loop(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    std::vector<rot8_bytecode> tmp = {rot8_bytecode::STP, rot8_bytecode::BIZ,
				      rot8_bytecode::BTP}; // >[<

    tape.insert(tape.end(), tmp.begin(), tmp.end());
}

void expand_endloop(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    std::vector<rot8_bytecode> tmp = {rot8_bytecode::STP, rot8_bytecode::FLB,
				      rot8_bytecode::ROR, rot8_bytecode::RNZ,
				      rot8_bytecode::BTP}; // >-+]<
    tape.insert(tape.end(), tmp.begin(), tmp.end());
}

void expand_tp(std::vector<rot8_bytecode> &tape, R8Operand arg,
	       bool count_offset = true) {
    r8asm_data opr = read_opr(arg);

    unit_offset_type offset = (unit_offset_type)opr - (unit_offset_type)dataptr;
    bool stepforward = (offset > 0);
    offset = abs(offset);
    for (unit_offset_type i = 0; i < offset; i++) {
	tape.push_back(stepforward ? rot8_bytecode::STP : rot8_bytecode::BTP);
    }
    if (count_offset)
	dataptr = opr;
}

void expand_anchor(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    if (const std::string *name = std::get_if<std::string>(&arg))
	datas.insert({*name, dataptr});
    else if (const r8asm_data *name = std::get_if<r8asm_data>(&arg))
	datas.insert({std::to_string(*name), dataptr});
}

void expand_erase(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    std::vector<rot8_bytecode> tmp = {rot8_bytecode::BIZ, rot8_bytecode::FLB,
				      rot8_bytecode::ROR,
				      rot8_bytecode::RNZ}; // [-+]
    for (unit_size_type i = 0; i < UNIT_SIZE; i++)
	tape.insert(tape.end(), tmp.begin(), tmp.end());
    return;
}

void expand_bytecode(std::vector<rot8_bytecode> &tape, R8Operand arg,
		     rot8_bytecode op, bool count_offset = true) {
    r8asm_data repeat = read_opr(arg, 1);
    for (r8asm_data i = 0; i < repeat; i++) {
	tape.push_back(op);
	if (count_offset) {
	    if (op == rot8_bytecode::STP)
		dataptr++;
	    else if (op == rot8_bytecode::BTP)
		dataptr--;
	}
    }
}

std::vector<rot8_bytecode> r8asm_tape_out(std::span<const R8Instruction> ops) {
    std::vector<rot8_bytecode> tape;
    for (const auto it : ops) {
	if (const rot8_bytecode *bc = std::get_if<rot8_bytecode>(&it.op)) {
	    expand_bytecode(tape, it.formal_para, *bc);
	} else if (const r8asm_builtin *builtin =
		       std::get_if<r8asm_builtin>(&it.op))
	    switch (*builtin) {
	    case r8asm_builtin::LOOP:
		expand_loop(tape, it.formal_para);
		break;
	    case r8asm_builtin::ENDLOOP:
		expand_endloop(tape, it.formal_para);
		break;
	    case r8asm_builtin::TP:;
		expand_tp(tape, it.formal_para);
		break;
	    case r8asm_builtin::XOR:
		expand_xor(tape, it.formal_para);
		break;
	    case r8asm_builtin::ANCHOR:
		expand_anchor(tape, it.formal_para);
	    case r8asm_builtin::ERASE:
		break;
		expand_erase(tape, it.formal_para);
		break;
	    default:;
	    }
    }
    return (tape);
}
