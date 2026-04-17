#include "r8asm/r8asm.hh"
#include <variant>

uint8_t UNIT_SIZE = 8;

std::map<std::string, int> labels = {{std::string("main"), 0}};
std::map<std::string, int> datas = {{std::string("_UNIT_SIZE"), UNIT_SIZE}};

void expand_xor(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    if (const std::string *operand = std::get_if<std::string>(&arg)) {
    } // TODO
}

void expand_loop(std::vector<rot8_bytecode> &tape, R8Operand arg) {
    tape.insert(tape.end(), {
				rot8_bytecode::STP,
			    });
}

std::vector<rot8_bytecode> r8asm_tape_out(std::span<const R8Instruction> ops) {
    std::vector<rot8_bytecode> tape;
    for (const auto it : ops) {
	int repeat = 1;
	if (const rot8_bytecode *bc = std::get_if<rot8_bytecode>(&it.op)) {
	    if (const std::string *operand = std::get_if<std::string>(&it.arg))
		repeat = datas[*operand];
	    else if (const int *operand = std::get_if<int>(&it.arg))
		repeat = *operand;
	    for (int i = 0; i < repeat; i++)
		tape.push_back(*bc);
	} else if (const r8asm_macro *macro = std::get_if<r8asm_macro>(&it.op))
	    switch (*macro) {
	    case r8asm_macro::LOOP:
		expand_loop(tape, it.arg);
		break;
	    case r8asm_macro::ENDLOOP:;
		break;
	    case r8asm_macro::TP:;
		break;
	    case r8asm_macro::XOR:;
		break;
	    default:;
	    }
    }
    return (tape);
}
