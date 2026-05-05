#include "r8asm/r8asm_parse.hh"
#include <fstream>
#include <string>

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
    r8asm_src out_src;

    std::ifstream ifile;
    std::string line_buffer;
    ifile.open(filename, std::ios::in);
    while (std::getline(ifile, line_buffer)) {
	auto comment = line_buffer.find_first_of(';');
	if (comment != std::string::npos)
	    line_buffer = line_buffer.substr(0, comment);
	std::vector<std::string> ins_buffer;
	while (!line_buffer.empty())
	    ins_buffer.push_back(r8asm_peel(line_buffer));
	out_src.raw_src.push_back(ins_buffer);
    }
    ifile.close();
    return (out_src);
}