#include "r8asm/r8asm_parse.hh"
#include "r8asm/r8asm_macro.hh"

#include <fstream>
#include <iterator>
#include <ranges>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::map<std::string, R8MetaOp> r8asm_parsemap = {
    {"%macro", r8asm_builtin::MACRO},
    {"%endmacro", r8asm_builtin::ENDMACRO},
    {"%repeat", r8asm_builtin::REPEAT},
    {"%endrepeat", r8asm_builtin::ENDREPEAT},
};

void r8asm_trim(std::string &str) {
    auto start = str.find_first_not_of(EMPTY_CHARS);

    if (!(start ==
	  std::string::npos)) { // The string isn't filled with empty chars.
	auto tmp_end = str.find_last_not_of(EMPTY_CHARS);
	str = str.substr(start, tmp_end - start + 1);
    } else
	str = "";
}

std::string r8asm_peel(std::string &str) {
    r8asm_trim(str);
    auto end = str.find_first_of(EMPTY_CHARS);

    if (end != std::string::npos) {
	std::string tmp_str = str.substr(0, end);
	str = str.substr(end);
	r8asm_trim(str);
	return (tmp_str);
    } else {
	std::string tmp_str = str;
	str = "";
	return (tmp_str);
    }
}

R8Src read_src(std::string filename) {
    R8Src src;

    std::ifstream ifile;
    ifile.open(filename, std::ios::in);

    std::string line_buffer;
    std::vector<std::string> ins_buffer;
    std::stack<r8asm_src_point> matcher;
    while (std::getline(ifile, line_buffer)) {
	ins_buffer.clear();
	auto comment = line_buffer.find_first_of(';');
	if (comment != std::string::npos)
	    line_buffer = line_buffer.substr(0, comment);
	if (line_buffer.empty())
	    continue;
	while (!line_buffer.empty())
	    ins_buffer.push_back(r8asm_peel(line_buffer));
	src.raw_src.push_back({ins_buffer});
	if (ins_buffer[0][0] == '%') {
	    if (ins_buffer[0].substr(1, 3) == "end") {
		src.codeblocks.push(
		    {matcher.top(), std::prev(src.raw_src.end())});
		matcher.pop();
	    } else
		matcher.push(--src.raw_src.end());
	}
    }
    ifile.close();
    return (src);
}

R8Operand r8asm_arg_stoi(std::string raw_arg) {
    try {
	r8asm_data arg = std::stoi(raw_arg);
	return (arg);
    } catch (std::invalid_argument) {
	if (datas.find(raw_arg) != datas.end())
	    return (datas[raw_arg]);
	return (raw_arg);
    }
}

R8Instruction
r8asm_translate_single_ins(std::vector<std::string> const raw_ins) {
    R8MetaOp tmp_op;
    if (r8asm_insmap.find(raw_ins[0]) != r8asm_insmap.end())
	if (raw_ins.size() == 1)
	    return (R8Instruction{r8asm_insmap[raw_ins[0]]});
	else
	    return (R8Instruction{r8asm_insmap[raw_ins[0]],
				  r8asm_arg_stoi(raw_ins[1])});
    else if (r8asm_macromap.find(raw_ins[0]) != r8asm_macromap.end()) {
	std::vector<R8Operand> tmp_operands;
	for (auto i : raw_ins | std::views::drop(1))
	    tmp_operands.push_back(r8asm_arg_stoi(i));
	return (R8Instruction{r8asm_macrocall{raw_ins[0], tmp_operands}});
    }
    return (R8Instruction{});
}

void R8Src::preprocess() {
    std::vector<R8Instruction> codeblock;
    while (!this->codeblocks.empty()) {
	auto i = this->codeblocks.top();
	this->codeblocks.pop();
	codeblock.clear();
	auto raw_codeblock =
	    std::ranges::subrange(std::next(i.first), i.second);
	/*
	    src.raw_src: 0 1 2 3(%macro) 4 5 6 7(%endmacro) 8
	    i:-----------------^first----------^second-------
	    subrange:--------------------4 5 6---------------
	*/
	for (auto j : raw_codeblock)
	    codeblock.push_back(r8asm_translate_single_ins(j));
	if ((*i.first)[0] == "%macro") {
	    r8asm_macromap.insert_or_assign(
		(*i.first)[1], R8Macro{(argc_type)std::get<r8asm_data>(
					   r8asm_arg_stoi((*i.first)[2])),
				       codeblock});
	} else if ((*i.first)[0] == "%repeat") {
	    for (auto count =
		     std::get<r8asm_data>(r8asm_arg_stoi((*i.first)[1]));
		 count > 0; count--)
		this->raw_src.insert(std::next(i.second), std::next(i.first),
				     i.second);
	}
	this->raw_src.erase(i.first, std::next(i.second));
    }
}

std::vector<R8Instruction> r8asm_preprocess(R8Src &src) {
    std::vector<R8Instruction> tmp_ins;
    src.preprocess();
    for (auto i : src.raw_src) {
	tmp_ins.push_back(r8asm_translate_single_ins(i));
    }
    return (tmp_ins);
}