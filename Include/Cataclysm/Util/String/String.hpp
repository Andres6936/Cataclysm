#ifndef _STRINGFUNC_H_
#define _STRINGFUNC_H_

#include <vector>
#include <string>
#include <istream>
#include <string_view>

#define STD_DELIM "</>"

namespace Cataclysm
{

	/**
	 * Remove the color tags of text.
	 *
	 * @param _text Text to delete the color tags
	 * @return The text without color tags.
	 */
	std::string removeColorTags(std::string_view _text);
}

std::vector<std::string> break_into_lines(const std::string& text,
		int linesize);

std::string load_to_delim(std::istream& datastream, std::string delim);

std::string slurp_file(const std::string& filename);

std::string trim(const std::string& orig);

std::string no_caps(const std::string& orig);

std::string capitalize(const std::string& orig);

// Convert an int to a string
std::string itos(int num);

bool is_vowel(char ch);

#endif
