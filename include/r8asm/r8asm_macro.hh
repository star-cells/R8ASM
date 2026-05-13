#ifndef R8ASM_MACRO
#define R8ASM_MACRO

#include "r8asm/r8asm_core.hh"
#include <vector>
struct R8Macro {
    argc_type argc = 0;
    std::vector<R8Instruction> ins;
};
extern std::map<std::string, R8Macro> r8asm_macromap;

std::vector<R8Instruction> expand_single_macro(std::string macro_name,
    std::vector<R8Operand> argv);

std::vector<R8Instruction> expand_macros(std::vector<R8Instruction> ins);

#endif