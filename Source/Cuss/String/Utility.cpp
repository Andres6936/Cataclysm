// Joan Andrés (@Andres6936) Github.

#include <curses.h>
#include <Cuss/String/Utility.hpp>

#define KEY_ESC 27

using namespace cuss;

bool Utility::is_backspace(long ch)
{
	return (ch == KEY_BACKSPACE || ch == 127 || ch == 8);
}

std::string Utility::key_name(long ch)
{
	switch (ch)
	{
	case KEY_UP:
		return "UP";
	case KEY_RIGHT:
		return "RIGHT";
	case KEY_LEFT:
		return "LEFT";
	case KEY_DOWN:
		return "DOWN";
	case '\n':
		return "ENTER";
	case '\t':
		return "TAB";
	case KEY_ESC:
		return "ESC";
	case KEY_BACKSPACE:
	case 127:
	case 8:
		return "BACKSPACE";
	default:
		if (ch < 256)
		{
			std::stringstream ret;
			ret << char(ch);
			return ret.str();
		}
		else
		{
			std::stringstream ret;
			ret << "[" << int(ch) << "]";
			return ret.str();
		}
	}
	return "???";
}

std::string Utility::removeColorTags(std::string_view _text)
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

std::vector<std::string> Utility::break_into_lines(const std::string& text, int linesize)
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

std::string Utility::load_to_delim(std::istream& datastream, std::string delim)
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
