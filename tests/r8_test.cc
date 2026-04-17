#include "r8asm/r8asm.hh"
#include <iostream>
int main() {
    std::vector<R8Instruction> ins = {{rot8_bytecode::STP, 3}};
    std::vector<rot8_bytecode> tape = r8asm_tape_out(ins);
    for (const auto i : tape)
	std::cout << ROT8_BC_TO_CHAR(i);
    return (0);
}
