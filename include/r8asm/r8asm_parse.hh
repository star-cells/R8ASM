#ifndef R8ASM_PARSE
#define R8ASM_PARSE

#include <string>
#include <vector>

#define EMPTY_CHARS (" \t\n\r\f\v")

struct r8asm_src {
    std::vector<std::vector<std::string>> raw_src;
};

r8asm_src read_src(std::string filename);

void include_src(r8asm_src);

#endif