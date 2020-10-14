// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Cuss/TextBox.hpp"
#include <Cuss/String/Utility.hpp>

using namespace cuss;

#define LINE_XOXO 4194424
#define LINE_OXOX 4194417
#define LINE_XXOO 4194413
#define LINE_OXXO 4194412
#define LINE_OOXX 4194411
#define LINE_XOOX 4194410
#define LINE_XXXO 4194420
#define LINE_XXOX 4194422
#define LINE_XOXX 4194421
#define LINE_OXXX 4194423
#define LINE_XXXX 4194414

#define SELECTCOLOR c_blue

void print_scrollbar(std::shared_ptr<Window> win, int posx, int posy, int length, int offset,
		int size, bool selected)
{
	nc_color barcol = (selected ? SELECTCOLOR : c_ltgray);
	int barsize = (length >= size ? -1 : 1 + ((length * length) / size));

	if (barsize == -1)
	{
// Don't print a scroll bar
		for (int y = posy; y < posy + length; y++)
			win->putch(posx, y, barcol, c_black, LINE_XOXO);
	}
	else
	{
		int barpos = (offset * length) / size;
		if (barpos + barsize > length)
			barpos = length - barsize;
		for (int y = 0; y < length; y++)
		{
			long ch = ((y >= barpos && y < barpos + barsize) ? '#' : LINE_XOXO);
			win->putch(posx, posy + y, barcol, c_black, ch);
		}
	}
}

void ele_textbox::draw(Doryen::Console& _console)
{
	std::vector<std::string> broken = Utility::break_into_lines(*text, sizex);

	// TODO: Clear Area
	//win->clear_area(posx, posy, posx + sizex - 1, posy + sizey - 1);

	for (int i = 0; i + offset <= broken.size() && i < sizey; i++)
	{
		int ypos, index;
		if (v_align == ALIGN_BOTTOM)
		{
			ypos = posy + sizey - 1 - i;
			index = broken.size() - 1 - i - offset;
		}
		else
		{ // Default to top-aligned
			ypos = posy + i;
			index = i + offset;
		}
		if (index >= 0 && index < broken.size())
		{
			std::vector<std::string> segments;
			std::vector<long> color_pairs;

			const auto colors = Utility::parseColorTags(broken[index], segments, color_pairs, fg, bg);

			std::uint32_t positionX = posx;
			std::uint32_t indexActualString = 0;

			for (const auto& [foreground, background] : colors)
			{
				_console.setForegroundColor(foreground);
				_console.setBackgroundColor(background);

				_console.write(positionX, ypos, segments[indexActualString]);

				// Move the position of x for avoid overlap
				positionX += segments[indexActualString].size();
				// Move to next string to print
				indexActualString += 1;
			}
		}
	}

//	if (selectable)
//		print_scrollbar(win, posx + sizex - 1, posy, sizey, offset, broken.size(),selected);
}

// *** TEXTBOX ELEMENT ***
void ele_textbox::draw(std::shared_ptr<Window> win)
{
	std::vector<std::string> broken = Utility::break_into_lines(*text, sizex);

	win->clear_area(posx, posy, posx + sizex - 1, posy + sizey - 1);

	for (int i = 0; i + offset <= broken.size() && i < sizey; i++)
	{
		int ypos, index;
		if (v_align == ALIGN_BOTTOM)
		{
			ypos = posy + sizey - 1 - i;
			index = broken.size() - 1 - i - offset;
		}
		else
		{ // Default to top-aligned
			ypos = posy + i;
			index = i + offset;
		}
		if (index >= 0 && index < broken.size())
		{
			if (align == ALIGN_RIGHT)
			{
				win->putstr_r(posx, ypos, fg, bg, sizex, broken[index]);
			}
			else if (align == ALIGN_CENTER)
			{
				win->putstr_c(posx, ypos, fg, bg, sizex, broken[index]);
			}
			else
			{
				win->putstr_n(posx, ypos, fg, bg, sizex, broken[index]);
			}
		}
	}

	if (selectable)
		print_scrollbar(win, posx + sizex - 1, posy, sizey, offset, broken.size(),
				selected);
}

std::string ele_textbox::save_data()
{
	std::stringstream ret;
	ret << element::save_data() << " " << (*text) << " " << STD_DELIM;

	return ret.str();
}

void ele_textbox::load_data(std::istream& datastream)
{
	element::load_data(datastream);
	(*text) = Utility::load_to_delim(datastream, STD_DELIM);
}

bool ele_textbox::self_reference()
{
	if (owns_data)
		return false;

	text = new std::string;
	owns_data = true;
	return true;
}

bool ele_textbox::set_data(std::string data)
{
	(*text) = data;
	return true;
}

bool ele_textbox::add_data(std::string data)
{
	(*text) += data;
	return true;
}

bool ele_textbox::set_data(std::vector<std::string> data)
{
	(*text) = "";
	add_data(data);
	return true;
}

bool ele_textbox::add_data(std::vector<std::string> data)
{
	for (int i = 0; i < data.size(); i++)
		(*text) += data[i] + '\n';
	return true;
}

bool ele_textbox::ref_data(std::string* data)
{
	if (owns_data)
		delete text;

	text = data;
	owns_data = false;
	return true;
}

bool ele_textbox::set_data(int data)
{
	std::vector<std::string> broken = Utility::break_into_lines(*text, sizex);
	if (data <= 0)
		offset = 0;
	else if (data > broken.size() - 1)
		offset = broken.size() - 1;
/*
 else if (data > sizey - broken.size())
  offset = sizey - broken.size();
*/
	else
		offset = data;
	return true;
}

bool ele_textbox::add_data(int data)
{
	return set_data(offset + data);
}

std::vector<std::string> ele_textbox::get_str_list()
{
	return Utility::break_into_lines(*text, sizex);
}
