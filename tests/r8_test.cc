#include <iostream>

#include "r8asm/r8asm.hh"
#include "r8asm/r8asm_parse.hh"

int main(int argc, char *argv[]) {
    auto lib = read_src("/home/admin/Git/R8ASM/asm/lib.r8asm");
    /*
    for (auto i : lib.raw_src)
	for (auto j : i)
	    std::cout << j << ';';*/
    lib.preprocess();
    auto src = read_src("asm/asm.r8asm");
    auto out_tape = r8asm_tape_out(expand_macros(r8asm_preprocess(src)));
    for (auto i : out_tape)
	std::cout << ROT8_BC_TO_CHAR(i);
    return (0);
}
