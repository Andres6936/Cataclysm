#include <sstream>
#include "Cataclysm/Util/String/stringfunc.h"

std::vector<std::string> break_into_lines(const std::string& text, int linesize)
{
	std::vector<std::string> ret;
	std::string line;
	std::string color_tag;
	size_t pos = 0; // Current position
	int line_length = 0;  // Non-color-tag characters in current line
	bool done = false;

/*
std::string linedraw;
for (int i = 0; i < linesize; i++) {
linedraw += '_';
}
*/

	while (!done)
	{

//debugmsg("pos %d (%s...)", pos, text.substr(pos, 8).c_str());

		size_t next_break = text.find_first_of(" \n", pos + 1);
		size_t next_tag = text.find("<c=", pos);

		if (next_break == std::string::npos)
		{ // No more whitespace!
			done = true;
			next_break = text.size() + 1; // This is okay, right?
		}

		std::string word = text.substr(pos, next_break - pos);
		int word_length = word.length();

		if (next_tag < next_break)
		{ // Subtract the color tag's length.
			size_t another_tag;
			do
			{
				size_t tag_end = text.find(">", next_tag);
				if (tag_end < next_break)
				{ // Ensure it's a real color tag!
					color_tag = text.substr(next_tag, tag_end - next_tag + 1);
					word_length -= color_tag.length();
				}
				another_tag = text.find("<c=", next_tag + 1);
				if (another_tag < next_break)
				{
					next_tag = another_tag;
				}
			} while (another_tag < next_break);
		}

//debugmsg("linesize %d\nsize |%s|\npos %d\nline |%s|, length %d\nword |%s|, length %d", linesize, linedraw.c_str(), pos, line.c_str(), line_length, word.c_str(), word_length);

		if (word_length > linesize)
		{ // Gonna have to hyphenate it.
//debugmsg("Word |%s|, length %d, linesize %d", word.c_str(), word_length, linesize);
			int chars_left = linesize - line_length;
			line += word.substr(0, chars_left - 1); // - 1 to make room for "-"
			line += "-";
//debugmsg("Pushing -%s-", line.c_str());
			ret.push_back(line);
			line = color_tag;
//debugmsg("Fresh line [%s]", line.c_str());
			pos += chars_left - 1; // Land on the letter "replaced" by "-"
			line_length = 0;
		}
		else
		{

// Word WILL fit in a single line.  Will it fit in THIS line?
			if (word.size() > 1 || word[0] != '\n')
			{
				if (line_length + word_length <= linesize)
				{ // Okay to add the word!
					if (word[0] == '\n')
					{
						word = word.substr(1);
						if (word_length > 1)
						{
							word_length--;
						}
					}
					line += word; // Includes the space before the word, if there is one.
					line_length += word_length;

				}
				else
				{ // Too long!  So start a new line.
//debugmsg("Pushing ^%s^", line.c_str());
					ret.push_back(line);
					line = color_tag; // Always start with the current color tag.
					if (!word.empty() && (word[0] == ' ' || word[0] == '\n'))
					{
						word = word.substr(1);
						if (word_length > 1)
						{
							word_length--;
						}
					}
					line += word;
					line_length = word_length;
//debugmsg("Fresh line [%s]", line.c_str());
				}
			}

			pos = next_break;

			if (text[pos] == '\n')
			{ // A forced break!
//debugmsg("Pushing ~%s~", line.c_str());
				ret.push_back(line);
				line = color_tag;
				//pos++;  // Skip over the '\n'.
				line_length = 0;
//debugmsg("Fresh line [%s]", line.c_str());
			}
		}
	}

	if (!line.empty())
	{
		ret.push_back(line);
	}

	return ret;
}

std::string load_to_delim(std::istream& datastream, std::string delim)
{
	std::string ret, tmp;
	do
	{
		datastream >> tmp;
		if (tmp != delim)
			ret += tmp + " ";
	} while (tmp != delim && !(datastream.eof()));

	if (!ret.empty() && ret[ret.size() - 1] == ' ')
		ret = ret.substr(0, ret.size() - 1);

	return ret;
}

std::string trim(const std::string& orig)
{
	std::string ret = orig;
	int front = 0, back = ret.length() - 1;
	while (front < ret.length() &&
		   (ret[front] == ' ' || ret[front] == '\n' || ret[front] == '\t'))
	{
		front++;
	}

	ret = ret.substr(front);

	back = ret.length() - 1;

	while (back >= 0 &&
		   (ret[back] == ' ' || ret[back] == '\n' || ret[back] == '\t'))
	{
		back--;
	}

	ret = ret.substr(0, back + 1);

	return ret;
}

std::string no_caps(const std::string& orig)
{
	std::string ret = orig;
	for (int i = 0; i < ret.length(); i++)
	{
		if (ret[i] >= 'A' && ret[i] <= 'Z')
			ret[i] += 'a' - 'A';
	}

	return ret;
}

std::string capitalize(const std::string& orig)
{
	std::string ret = orig;
	size_t tagpos = orig.find("<c="); // Find the first tag
	size_t start; // Used below
	if (tagpos != std::string::npos)
	{
		for (int i = 0; i < tagpos; i++)
		{  // Can we capitalize before the tag?
			if (ret[i] >= 'a' && ret[i] <= 'z')
			{
				ret[i] += 'A' - 'a';  // Capitalize!
				return ret;
			}
			else if (ret[i] != ' ')
			{
				return ret; // We're already capitalized!
			}
		}
// If we reach this point, we found a tag but there's nothing before it.
		start = orig.find(">", tagpos);
		start++;
	}
	else
	{  // No tags - start from the beginning of the string
		start = 0;
	}
	for (int i = start; i < ret.size(); i++)
	{
		if (ret[i] >= 'a' && ret[i] <= 'z')
		{
			ret[i] += 'A' - 'a';
			return ret;
		}
		else if (ret[i] != ' ')
		{
			return ret; // We're already capitalized!
		}
	}
	return ret; // All blank spaces??
}

std::string remove_color_tags(const std::string& _text)
{
	std::string result;

	// Preallocate the estimated amount of storage.
	// This improves the cache locality of the data accessed.
	result.reserve(_text.size());

	bool in_tag = false;

	// Eliminate Pointer Dereference Using Iterators
	for (const auto& character : _text)
	{
		if (in_tag)
		{
			if (character == '>')
			{
				in_tag = false;
			}
		}
		else
		{
			if (character == '<')
			{
				in_tag = true;
			}
			else
			{
				result += character;
			}
		}
	}

	return result;
}

std::string itos(int num)
{
	std::stringstream ret;
	ret << num;
	return ret.str();
}

bool is_vowel(char ch)
{
	return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
			ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U');
}
