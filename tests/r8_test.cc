#include <iostream>

#include "r8asm/r8asm.hh"

int main()
{
    std::vector<R8Instruction> ins = {
	{ rot8_bytecode::INP }, { r8asm_macro::LOOP },
	{ rot8_bytecode::BIZ }, { rot8_bytecode::FLB },
	{ rot8_bytecode::RNZ }, { rot8_bytecode::ROR },
	{ r8asm_macro::ENDLOOP }, { rot8_bytecode::OUT }
    };
    std::vector<rot8_bytecode> tape = r8asm_tape_out(ins);

    for (const auto i : tape)
	std::cout << ROT8_BC_TO_CHAR(i);
    return (0);
}
