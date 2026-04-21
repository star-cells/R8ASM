#include <iostream>

#include "r8asm/r8asm.hh"
#include "r8asm/r8asm_core.hh"

int main()
{
    std::vector<R8Instruction> ins = {
	R8Instruction { rot8_bytecode::INP }, // To make clang-format distribute all
	// instructions into diffirent lines.
	{ r8asm_builtin::LOOP },
	{ rot8_bytecode::BIZ },
	{ rot8_bytecode::FLB },
	{ rot8_bytecode::RNZ },
	{ rot8_bytecode::ROR },
	{ r8asm_builtin::ENDLOOP },
    };
    std::vector<rot8_bytecode> tape = r8asm_tape_out(ins);

    for (const auto i : tape)
	std::cout << ROT8_BC_TO_CHAR(i);
    return (0);
}
