#ifndef _STRINGFUNC_H_
#define _STRINGFUNC_H_

#include <vector>
#include <string>
#include <istream>
#include <string_view>

#define STD_DELIM "</>"

std::string slurp_file(const std::string& filename);

std::string trim(const std::string& orig);

std::string no_caps(const std::string& orig);

std::string capitalize(const std::string& orig);

bool is_vowel(char ch);

#endif
