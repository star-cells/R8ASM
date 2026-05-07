#include "r8asm/r8asm_parse.hh"
#include "r8asm/r8asm_core.hh"
#include <fstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

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

r8asm_src read_src(std::string filename) {
    r8asm_src src;

    ins_type_sign line_counter = 0;
    std::ifstream ifile;
    std::string line_buffer;
    ifile.open(filename, std::ios::in);
    std::vector<std::string> ins_buffer;
    std::stack<ins_type_sign> matcher;
    while (std::getline(ifile, line_buffer)) {
	ins_buffer.clear();
	auto comment = line_buffer.find_first_of(';');
	if (comment != std::string::npos)
	    line_buffer = line_buffer.substr(0, comment);
	if (line_buffer.empty())
	    continue;
	line_counter++;
	while (!line_buffer.empty())
	    ins_buffer.push_back(r8asm_peel(line_buffer));
	src.raw_src.push_back({ins_buffer});
	if (ins_buffer[0][0] == '%') {
	    if (ins_buffer[0].substr(1, 3) == "end") {
		src.codeblocks.push_back({matcher.top(), line_counter});
		matcher.pop();
	    } else
		matcher.push(line_counter);
	}
    }
    ifile.close();
    return (src);
}

std::vector<R8Instruction> r8asm_preprocess(r8asm_src &src) {
    std::vector<R8Instruction> ins;
}