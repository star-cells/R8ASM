#include "r8asm/r8asm_core.hh"
unit_size_type UNIT_SIZE = DEFAULT_UNIT_SIZE;
mem_size_type dataptr = 0;

std::map<std::string, r8asm_label> labels = {{std::string("main"), 0}};
std::map<std::string, r8asm_data> datas = {
    {std::string("_UNIT_MAX"), DEFAULT_UNIT_MAX},
    {std::string{"_UNIT_SIZE"}, DEFAULT_UNIT_SIZE}};

std::map<std::string, R8MetaOp> r8asm_insmap = {
    {"stp", rot8_bytecode::STP},	{"btp", rot8_bytecode::BTP},
    {"ror", rot8_bytecode::ROR},	{"flb", rot8_bytecode::FLB},
    {"biz", rot8_bytecode::BIZ},	{"rnz", rot8_bytecode::RNZ},
    {"out", rot8_bytecode::OUT},	{"inp", rot8_bytecode::INP},
    {".loop", r8asm_builtin::LOOP},	{".endloop", r8asm_builtin::ENDLOOP},
    {".anchor", r8asm_builtin::ANCHOR}, {".tp", r8asm_builtin::TP},
    {".xor", r8asm_builtin::XOR},
};
