#include "r8asm/r8asm_macro.hh"
#include <variant>
#include <vector>

std::map<std::string, R8Macro> r8asm_macromap;

R8Operand read_arg(std::vector<R8Operand> &actual_args, R8Operand formal_para,
		   r8asm_argc argc) {
    if (std::string *str = std::get_if<std::string>(&formal_para)) {
	if ((*str) == "%0")
	    return ((r8asm_data)argc);
	else if ((*str)[0] == '%')
	    return (actual_args[std::stoi(str->substr(1)) - 1]);
	// e.g. "%15" -> "15" -> 14 -> argv[14]
    }
    return (formal_para);
}

std::vector<R8Instruction>
r8asm_expand_single_macro(std::string macro_name,
			  std::vector<R8Operand> actual_args) {
    std::vector<R8Instruction> tmp_tape;
    R8Macro this_macro = r8asm_macromap[macro_name];

    for (auto i : this_macro.ins) {
	if (std::get_if<rot8_bytecode>(&i.op) ||
	    std::get_if<r8asm_builtin>(&i.op))
	    tmp_tape.push_back(
		{i.op, read_arg(actual_args, i.formal_para, this_macro.argc)});
	else if (r8asm_macrocall *tmp_macro =
		     std::get_if<r8asm_macrocall>(&i.op)) {
	    std::vector<R8Operand> macro_args;
	    for (auto j : tmp_macro->actual_args)
		macro_args.push_back(read_arg(actual_args, j, this_macro.argc));
	    std::vector<R8Instruction> macro_body =
		r8asm_expand_single_macro(tmp_macro->name, macro_args);
	    tmp_tape.insert(tmp_tape.end(), macro_body.begin(),
			    macro_body.end());
	}
    }

    return (tmp_tape);
}

std::vector<R8Instruction> r8asm_expand_macros(std::vector<R8Instruction> ins) {
    std::vector<R8Instruction> tape;
    for (auto i : ins) {
	if (r8asm_macrocall *tmp_macro = std::get_if<r8asm_macrocall>(&i.op)) {
	    std::vector<R8Instruction> tmp_tape = (r8asm_expand_single_macro(
		tmp_macro->name, tmp_macro->actual_args));
	    tape.insert(tape.end(), tmp_tape.begin(), tmp_tape.end());
	} else
	    tape.push_back(i);
    }
    return (tape);
}