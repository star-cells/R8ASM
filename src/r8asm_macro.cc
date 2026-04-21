#include "r8asm/r8asm_macro.hh"

std::map<std::string, R8Macro> macros;

std::vector<R8Instruction> expand_macro(std::string macro_name,
    std::vector<R8Operand> argv)
{
    std::vector<R8Instruction> tmp_tape;

    // TODO:explain args

    for (auto i : macros[macro_name].ins) {
	if (std::get_if<rot8_bytecode>(&i.op) || std::get_if<r8asm_builtin>(&i.op))
	    tmp_tape.push_back(i);
	else if (r8asm_macro *macro = std::get_if<r8asm_macro>(&i.op)) {
	    std::vector<R8Instruction> expanded_macro = expand_macro(macro->name, macro->args);
	    tmp_tape.insert(tmp_tape.end(), expanded_macro.begin(), expanded_macro.end());
	}
    }

    return (tmp_tape);
}