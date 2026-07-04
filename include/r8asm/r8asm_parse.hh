#ifndef R8ASM_PARSE
#define R8ASM_PARSE

#include "r8asm/r8asm_core.hh"
#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define EMPTY_CHARS (" \t\n\r\f\v")

typedef std::list<std::vector<std::string>>::iterator r8asm_src_point;

struct R8Src {
    std::list<std::vector<std::string>> raw_src;
    std::queue<std::pair<r8asm_src_point, r8asm_src_point>> codeblocks;
    void preprocess_blocks();
    void preprocess_lines();
};

R8Src r8asm_read_src(std::string filename);
std::vector<R8Instruction> r8asm_preprocess(R8Src &src);
extern std::unordered_map<r8asm_parse, std::string> r8asm_parsemap;

#endif