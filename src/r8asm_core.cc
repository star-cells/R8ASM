#include "r8asm/r8asm_core.hh"
unit_size_type UNIT_SIZE = DEFAULT_UNIT_SIZE;
mem_size_type dataptr = 0;

std::map<std::string, r8asm_label> labels = { { std::string("main"), 0 } };
std::map<std::string, r8asm_data> datas = {
    { std::string("_UNIT_MAX"), UNIT_MAX }
};