#include <iostream>

#include "r8asm/r8asm.hh"
#include "r8asm/r8asm_parse.hh"

int main(int argc, char *argv[]) {
    auto lib = read_src("asm/lib.r8asm");
    auto r8and = read_src("asm/and.r8asm");
    auto r8or = read_src("asm/or.r8asm");
    auto r8xor = read_src("asm/xor.r8asm");
    auto r8add = read_src("asm/add.r8asm");
    /*
    for (auto i : lib.raw_src)
	for (auto j : i)
	    std::cout << j << ';';*/
    lib.preprocess();
    r8and.preprocess();
    r8or.preprocess();
    r8xor.preprocess();
    r8add.preprocess();
    auto src = read_src("asm/asm.r8asm");
    auto out_tape = r8asm_tape_out(expand_macros(r8asm_preprocess(src)));
    for (auto i : out_tape)
	std::cout << ROT8_BC_TO_CHAR(i);
    std::cout << std::endl;
    return (0);
}
