#include "r8asm/r8asm_macro.hh"
#include "r8asm/r8asm_core.hh"
#include <variant>
#include <vector>

std::map<std::string, R8Macro> macros = {
    {std::string("STEP"),
     {1,
      {{rot8_bytecode::STP, std::string("%1")},
       {rot8_bytecode::STP, std::string("%1")}}}},
    {std::string("BACK"),
     {1,
      {{rot8_bytecode::BTP, std::string("%1")},
       {rot8_bytecode::BTP, std::string("%1")}}}}};

R8Operand read_arg(std::vector<R8Operand> &argv, R8Operand argf,
		   argc_type argc) {
    if (std::string *str = std::get_if<std::string>(&argf)) {
	if ((*str) == "%0")
	    return ((r8asm_data)argc);
	else if ((*str)[0] == '%')
	    return (argv[std::stoi(str->substr(1)) - 1]);
	// e.g. "%15" -> "15" -> 14 -> argv[14]
    }
    return (argf);
}

std::vector<R8Instruction> expand_single_macro(std::string macro_name,
					       std::vector<R8Operand> argv) {
    std::vector<R8Instruction> tmp_tape;

    for (auto i : macros[macro_name].ins) {
	if (std::get_if<rot8_bytecode>(&i.op) ||
	    std::get_if<r8asm_builtin>(&i.op))
	    tmp_tape.push_back(
		{i.op, read_arg(argv, i.arg, macros[macro_name].argc)});
	else if (r8asm_macro *macro = std::get_if<r8asm_macro>(&i.op)) {
	    std::vector<R8Operand> macro_args;
	    for (auto j : macro->args)
		macro_args.push_back(
		    read_arg(argv, j, macros[macro_name].argc));
	    std::vector<R8Instruction> macro_body =
		expand_single_macro(macro->name, macro_args);
	    tmp_tape.insert(tmp_tape.end(), macro_body.begin(),
			    macro_body.end());
	}
    }

    return (tmp_tape);
}

std::vector<R8Instruction> expand_macros(std::vector<R8Instruction> &ins) {
    std::vector<R8Instruction> out_tape;
    for (auto i : ins) {
	if (r8asm_macro *macro = std::get_if<r8asm_macro>(&i.op)) {
	    std::vector<R8Instruction> tmp_tape =
		(expand_single_macro(macro->name, macro->args));
	    out_tape.insert(out_tape.end(), tmp_tape.begin(), tmp_tape.end());
	} else
	    out_tape.push_back(i);
    }
    return (out_tape);
}