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

std::vector<std::pair<Doryen::Color, Doryen::Color>> Utility::parseColorTags(
		std::string text, std::vector<std::string>& segments,
		std::vector<long>& color_pairs, nc_color fg, nc_color bg)
{
	std::vector<std::pair<Doryen::Color, Doryen::Color>> colors {};

	size_t tag;
	nc_color cur_fg = fg, cur_bg = bg;

	while ((tag = text.find("<c=")) != std::string::npos)
	{
		// Everything before the tag is a segment, with the current colors
		segments.push_back(text.substr(0, tag));
		color_pairs.push_back(get_color_pair(cur_fg, cur_bg));

		// Transform the Cataclysm color to Doryen color
		colors.push_back(Cataclysm::getColorPair(cur_fg, cur_bg));

		// Strip off everything up to and including "<c="
		text = text.substr(tag + 3);
		// Find the end of the tag
		size_t tagend = text.find(">");
		if (tagend == std::string::npos)
		{
			// debugmsg("Unterminated color tag! %d:%s:", int(tag), text.c_str());
			return colors;
		}
		std::string tag = text.substr(0, tagend);
		// Strip out the tag
		text = text.substr(tagend + 1);

		if (tag == "reset" || tag == "/")
		{ // Reset the colors
			cur_fg = fg;
			cur_bg = bg;
		}
		else
		{ // We're looking for the color!
			size_t comma = tag.find(",");
			if (comma == std::string::npos)
			{ // No comma - just setting fg
				cur_fg = color_string(tag);
				if (cur_fg == c_null)
				{
//					debugmsg("Malformed color tag: %s", tag.c_str());
					return colors;
				}
			}
			else
			{
				nc_color new_fg = color_string(tag.substr(0, comma)),
						new_bg = color_string(tag.substr(comma + 1));
				if (new_fg == c_null && new_bg == c_null)
				{
//					debugmsg("Malformed color tag: %s", tag.c_str());
					return colors;
				}
				if (new_fg != c_null)
					cur_fg = new_fg;
				if (new_bg != c_null)
					cur_bg = new_bg;
			} // if comma was found
		} // color needed to be found
	} // while (tag != std::string::npos)
	// There's a little string left over; push it into our vectors!
	segments.push_back(text);
	color_pairs.push_back(get_color_pair(cur_fg, cur_bg));

	// Transform the Cataclysm color to Doryen color
	colors.push_back(Cataclysm::getColorPair(cur_fg, cur_bg));

	if (segments.size() != color_pairs.size())
	{
//		debugmsg("Segments.size() = %d, color_pairs.size() = %d",segments.size(), color_pairs.size());
		return colors;
	}

	return colors;
}
