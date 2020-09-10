#ifndef _STRINGFUNC_H_
#define _STRINGFUNC_H_

#include <string>
#include <vector>
#include <istream>

#define STD_DELIM "</>"

std::vector<std::string> break_into_lines(const std::string& text,
		int linesize);

std::string load_to_delim(std::istream& datastream, std::string delim);

std::string slurp_file(const std::string& filename);

std::string trim(const std::string& orig);

std::string no_caps(const std::string& orig);

std::string capitalize(const std::string& orig);

std::string remove_color_tags(const std::string& _text);

// Convert an int to a string
std::string itos(int num);

bool is_vowel(char ch);

#endif
