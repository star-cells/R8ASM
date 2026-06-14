#ifndef R8ASM_CORE
#define R8ASM_CORE

#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

typedef uint32_t r8asm_label;
typedef int32_t r8asm_data;
typedef uint32_t cell_size_type;
typedef int32_t cell_offset_type;
typedef uint32_t mem_size_type;
typedef uint32_t argc_type;

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
    MACRO,
    ENDMACRO,
    REPEAT,
    ENDREPEAT,
    INCLUDE,
};

using R8Operand = std::variant<std::monostate, r8asm_data, std::string>;

struct r8asm_macrocall {
    std::string name;
    std::vector<R8Operand> actual_args;
};

using R8MetaOp = std::variant<r8asm_builtin, r8asm_macrocall, rot8_bytecode>;

struct R8Instruction {
    R8MetaOp op;
    R8Operand formal_para;
};

extern std::map<std::string, r8asm_label> labels;
extern std::map<std::string, r8asm_data> datas;
extern mem_size_type dataptr;

extern std::map<std::string, R8MetaOp> r8asm_insmap;

extern cell_size_type CELL_SIZE;
#define CELL_MAX (((uint64_t)1 << CELL_SIZE) - 1)

// #define DEFAULT_cell_SIZE (sizeof(cell_size_type) * 8)
#define DEFAULT_CELL_SIZE 8 // Just for test.
#define DEFAULT_CELL_MAX (((uint64_t)1 << DEFAULT_CELL_SIZE) - 1)

#endif