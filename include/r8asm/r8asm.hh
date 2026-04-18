#ifndef R8ASM
#define R8ASM

#include <cstdint>
#include <map>
#include <span>
#include <string>
#include <variant>
#include <vector>

#define ROT8_BC_TO_CHAR(bc) ("><+-[].,"[static_cast<int>(bc)])

enum class rot8_bytecode { STP, BTP, ROR, FLB, BIZ, RNZ, OUT, INP };
enum class r8asm_macro {
    LOOP,
    ENDLOOP,
    TP,
    XOR,
};

using R8Operand = std::variant<std::monostate, int, std::string>;
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

extern std::map<std::string, int> labels;
extern std::map<std::string, int> datas;
extern std::map<std::string, std::vector<R8Macro>> macros;

extern uint8_t UNIT_SIZE;
#define UNIT_MAX ((1 << UNIT_SIZE) - 1)

#endif