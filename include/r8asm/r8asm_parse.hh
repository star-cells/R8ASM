#ifndef R8ASM_PARSE
#define R8ASM_PARSE

#include "r8asm/r8asm_core.hh"
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#define EMPTY_CHARS (" \t\n\r\f\v")

typedef size_t ins_type_sign;

struct r8asm_src {
    std::vector<std::vector<std::string>> raw_src;
    std::vector<std::pair<ins_type_sign, ins_type_sign>> codeblocks;
};

r8asm_src read_src(std::string filename);

void include_src(r8asm_src);

std::vector<R8Instruction> r8asm_preprocess(r8asm_src &src);

#endif