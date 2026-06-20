#include <iostream>

#include "r8asm/r8asm.hh"
#include "r8asm/r8asm_parse.hh"

int main(int argc, char *argv[]) {
    std::string src_filename;
    src_filename = argv[argc - 1];
    for (auto i = 1; i < argc - 1; i++) {
	auto r8lib = read_src(argv[i]);
	r8lib.preprocess();
    }
    auto src = read_src(src_filename.c_str());
    auto out_tape = r8asm_tape_out(expand_macros(r8asm_preprocess(src)));
    for (auto i : out_tape)
	std::cout << ROT8_BC_TO_CHAR(i);
    std::cout << std::endl;
    return (0);
}
