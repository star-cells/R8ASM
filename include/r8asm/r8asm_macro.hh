#ifndef R8ASM_MACRO
#define R8ASM_MACRO

#include "r8asm/r8asm_core.hh"
struct R8Macro {
    argc_type argc;
    std::vector<R8Instruction> ins;
};
extern std::map<std::string, R8Macro> macros;

std::vector<R8Instruction> expand_macro(std::string macro_name, std::vector<R8Operand> argv);

#endif