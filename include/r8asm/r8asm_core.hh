#ifndef R8ASM_CORE
#define R8ASM_CORE

#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

using r8asm_label = uint32_t;
using r8asm_cell_size = uint8_t;
using r8asm_cell_offset = int32_t;
using r8asm_mem_size = uint32_t;
using r8asm_argc = r8asm_mem_size;

using r8asm_data = int32_t;

#define ROT8_BC_TO_CHAR(bc) ("><+-[].,"[static_cast<int>(bc)])

enum class rot8_bytecode {
    STP,
    BTP,
    ROR,
    FLB,
    BIZ,
    RNZ,
    OUT,
    INP,
};
enum class r8asm_builtin {
    LOOP,
    ENDLOOP,
    TP,
    XOR,
    ANCHOR,
    ERASE,
};

enum class r8asm_parse {
    MACRO,
    ENDMACRO,
    REPEAT,
    ENDREPEAT,
    INCLUDE,
    VAR,
};

using R8Operand = std::variant<std::monostate, r8asm_data, std::string>;

struct r8asm_macrocall {
    std::string name;
    std::vector<R8Operand> actual_args;
};

using R8MetaOp =
    std::variant<r8asm_builtin, r8asm_macrocall, rot8_bytecode, r8asm_parse>;

struct R8Instruction {
    R8MetaOp op;
    R8Operand formal_para;
};

extern std::map<std::string, r8asm_label>
    r8asm_labelmap; // TODO:sections support
extern std::map<std::string, r8asm_data> r8asm_datamap;
extern r8asm_mem_size rot8_dataptr;

extern std::map<std::string, R8MetaOp> r8asm_insmap;

extern r8asm_cell_size CELL_SIZE;
#define CELL_MAX (((uint64_t)1 << CELL_SIZE) - 1)

#define DEFAULT_CELL_SIZE 8 // Just for test.
#define DEFAULT_CELL_MAX (((uint64_t)1 << DEFAULT_CELL_SIZE) - 1)

#endif