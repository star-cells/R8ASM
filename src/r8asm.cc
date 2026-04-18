#include "r8asm/r8asm.hh"

#include <variant>
#include <vector>

unit_size_type UNIT_SIZE = DEFAULT_UNIT_SIZE;

std::map<std::string, r8asm_label> labels = {{std::string("main"), 0}};
std::map<std::string, r8asm_data> datas = {
    {std::string("_UNIT_MAX"), UNIT_MAX}};

r8asm_data read_opr(R8Operand arg, r8asm_data default_opr = 1) {
    if (const std::string *operand = std::get_if<std::string>(&arg))
	return (datas[*operand]);
    else if (const r8asm_data *operand = std::get_if<r8asm_data>(&arg))
	return (*operand);
    return (default_opr);
}

void expand_xor(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    r8asm_data opr = read_opr(arg);

    for (unit_size_type i = 0; i < UNIT_SIZE; i++) {
	if ((opr & 1) == 1) tape.push_back(rot8_bytecode::FLB);
	tape.push_back(rot8_bytecode::ROR);
	opr >>= 1;
    }
}

void expand_loop(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    r8asm_data opr = read_opr(arg, UNIT_MAX);

    tape.push_back(rot8_bytecode::STP);
    expand_xor(tape, opr);
    tape.push_back(rot8_bytecode::BIZ);
    tape.push_back(rot8_bytecode::BTP);	 // >(xor)[<
}

void expand_endloop(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    std::vector<rot8_bytecode> tmp = {rot8_bytecode::STP, rot8_bytecode::FLB,
				      rot8_bytecode::ROR, rot8_bytecode::RNZ,
				      rot8_bytecode::BTP};  // >-+]<
    tape.insert(tape.end(), tmp.begin(), tmp.end());
}

std::vector<rot8_bytecode> r8asm_tape_out(std::span<const R8Instruction> ops) {
    std::vector<rot8_bytecode> tape;
    for (const auto it : ops) {
	if (const rot8_bytecode *bc = std::get_if<rot8_bytecode>(&it.op)) {
	    r8asm_data repeat = read_opr(it.arg);
	    for (r8asm_data i = 0; i < repeat; i++) tape.push_back(*bc);
	} else if (const r8asm_macro *macro = std::get_if<r8asm_macro>(&it.op))
	    switch (*macro) {
		case r8asm_macro::LOOP:
		    expand_loop(tape, it.arg);
		    break;
		case r8asm_macro::ENDLOOP:;
		    expand_endloop(tape, it.arg);
		    break;
		case r8asm_macro::TP:;
		    break;
		case r8asm_macro::XOR:;
		    expand_xor(tape, it.arg);
		    break;
		default:;
	    }
    }
    return (tape);
}
