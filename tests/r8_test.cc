#include <iostream>

#include "r8asm/r8asm.hh"

int main(int argc, char *argv[]) {
    auto src = read_src(argc > 1 ? argv[1] : "asm/asm.r8asm");
    auto out_tape = r8asm_tape_out(expand_macros(r8asm_preprocess(src)));
    for (auto i : out_tape)
	std::cout << ROT8_BC_TO_CHAR(i);
    return (0);
}
