#ifndef R8ASM_ASM
#define R8ASM_ASM

#include "r8asm/r8asm_core.hh"
#include <span>

std::vector<rot8_bytecode> r8asm_tape_out(std::span<const R8Instruction> ops);

#endif