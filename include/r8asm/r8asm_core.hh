#ifndef R8ASM_CORE
#define R8ASM_CORE

#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

typedef uint32_t r8asm_label;
typedef int32_t r8asm_data;
typedef uint32_t unit_size_type;
typedef int32_t unit_offset_type;
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
};

using R8Operand = std::variant<std::monostate, r8asm_data, std::string, std::vector<std::string>>;

struct r8asm_macro {
    std::string name;
    std::vector<R8Operand> args;
};

using R8MetaOp = std::variant<r8asm_builtin, r8asm_macro, rot8_bytecode>;

struct R8Instruction {
    R8MetaOp op;
    R8Operand arg;
    R8Instruction(R8MetaOp in_op, R8Operand in_arg = std::monostate {})
	: op(in_op)
	, arg(in_arg) {
	};
};

extern std::map<std::string, r8asm_label> labels;
extern std::map<std::string, r8asm_data> datas;
extern mem_size_type dataptr;

extern unit_size_type UNIT_SIZE;
// #define DEFAULT_UNIT_SIZE (sizeof(unit_size_type) * 8)
#define DEFAULT_UNIT_SIZE 8 // Just for test.
#define UNIT_MAX (((uint64_t)1 << UNIT_SIZE) - 1)

#endif