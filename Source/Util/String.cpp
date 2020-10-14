#include <sstream>
#include "Cataclysm/Util/String.hpp"

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

bool is_vowel(char ch)
{
	return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
			ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U');
}
