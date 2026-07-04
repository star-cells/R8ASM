#include "r8asm/r8asm_core.hh"
r8asm_cell_size CELL_SIZE = DEFAULT_CELL_SIZE;
r8asm_mem_size rot8_dataptr = 0;

std::map<std::string, r8asm_label> r8asm_labelmap = {{std::string("main"), 0}};
std::map<std::string, r8asm_data> r8asm_datamap = {
    {std::string("_CELL_MAX"), DEFAULT_CELL_MAX},
    {std::string{"_CELL_SIZE"}, DEFAULT_CELL_SIZE},
    {std::string("_ROL_STEP"), DEFAULT_ROL_STEP}};

std::map<std::string, R8MetaOp> r8asm_insmap = {
    {"stp", rot8_bytecode::STP},	{"btp", rot8_bytecode::BTP},
    {"ror", rot8_bytecode::ROR},	{"flb", rot8_bytecode::FLB},
    {"biz", rot8_bytecode::BIZ},	{"rnz", rot8_bytecode::RNZ},
    {"out", rot8_bytecode::OUT},	{"inp", rot8_bytecode::INP},
    {".loop", r8asm_builtin::LOOP},	{".endloop", r8asm_builtin::ENDLOOP},
    {".anchor", r8asm_builtin::ANCHOR}, {".tp", r8asm_builtin::TP},
    {".xor", r8asm_builtin::XOR},
};
