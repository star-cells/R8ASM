#ifndef R8ASM
#define R8ASM

#include <cstdint>
#include <map>
#include <span>
#include <string>
#include <variant>
#include <vector>

typedef uint32_t r8asm_label;
typedef uint32_t r8asm_data;
typedef uint32_t unit_size_type;

#define ROT8_BC_TO_CHAR(bc) ("><+-[].,"[static_cast<uint8_t>(bc)])

enum class rot8_bytecode { STP, BTP, ROR, FLB, BIZ, RNZ, OUT, INP };
enum class r8asm_macro {
    LOOP,
    ENDLOOP,
    TP,
    XOR,
};

using R8Operand = std::variant<std::monostate, r8asm_data, std::string>;
using R8MetaOp = std::variant<r8asm_macro, rot8_bytecode>;

struct R8Instruction {
    R8MetaOp op;
    R8Operand arg = std::monostate{};
};

class R8Macro {
    std::vector<R8Operand> args = {std::monostate{}};
    std::vector<R8Instruction> ins;
};

std::vector<rot8_bytecode> r8asm_tape_out(std::span<const R8Instruction> ops);

extern std::map<std::string, r8asm_label> labels;
extern std::map<std::string, r8asm_data> datas;
extern std::map<std::string, std::vector<R8Macro>> macros;

extern unit_size_type UNIT_SIZE;
// #define DEFAULT_UNIT_SIZE (sizeof(unit_size_type) * 8)
#define DEFAULT_UNIT_SIZE 17  // Just for test.
#define UNIT_MAX (((uint64_t)1 << UNIT_SIZE) - 1)

#endif