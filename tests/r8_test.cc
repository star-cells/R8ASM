#include <iostream>

#include "r8asm/r8asm.hh"
#include "r8asm/r8asm_core.hh"
#include "r8asm/r8asm_macro.hh"

int main() {
    std::vector<R8Instruction> ins = {
	R8Instruction{rot8_bytecode::INP},
	// To make clang-format distribute all instructions into diffirent
	// lines.
	{r8asm_macro("STEP"), {15}},
	{rot8_bytecode::OUT},
    };

    // for (const auto i : tape)
    // std::cout << ROT8_BC_TO_CHAR(i);
    return (0);
}
