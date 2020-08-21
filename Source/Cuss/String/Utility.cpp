// Joan Andrés (@Andres6936) Github.

#include <Cuss/String/Utility.hpp>

using namespace cuss;

std::string Utility::stripTags(std::string _text)
{
	std::size_t tag;
	std::string ret;
	while ((tag = _text.find("<c=")) != std::string::npos)
	{
		ret += _text.substr(0, tag);
		// Strip off everything up to and including "<c="
		_text = _text.substr(tag + 3);
		// Find the end of the tag
		size_t tagend = _text.find(">");
		if (tagend == std::string::npos)
		{
			// Couldn't find the end of the tag, so just return
			ret += _text;
			return ret;
		}
		_text = _text.substr(tagend + 1);
	}
	ret += _text;
	return ret;
}
