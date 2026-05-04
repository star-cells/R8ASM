#include <iostream>

#include "r8asm/r8asm.hh"
#include "r8asm/r8asm_asm.hh"
#include "r8asm/r8asm_core.hh"
#include "r8asm/r8asm_macro.hh"

int main() {
    std::vector<R8Instruction> ins = {
	R8Instruction{rot8_bytecode::INP},
	// To make clang-format distribute all instructions into diffirent
	// lines.
	{rot8_bytecode::STP},
	{r8asm_builtin::XOR, 255},
	{rot8_bytecode::BTP},
	{r8asm_builtin::LOOP},
	{rot8_bytecode::BIZ},
	{rot8_bytecode::FLB},
	{rot8_bytecode::RNZ},
	{rot8_bytecode::ROR},
	{rot8_bytecode::OUT},
	{r8asm_builtin::ENDLOOP},
    };
    auto out_tape = r8asm_tape_out(expand_macros(ins));

    for (auto i : out_tape)
	std::cout << ROT8_BC_TO_CHAR(i);
    return (0);
}
