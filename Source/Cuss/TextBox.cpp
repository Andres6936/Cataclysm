// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Cuss/TextBox.hpp"
#include <Cataclysm/Screen/Debugger.hpp>
#include <Cataclysm/stringfunc.h>

using namespace cuss;

#define SELECTCOLOR c_blue

void print_scrollbar(Window* win, int posx, int posy, int length, int offset,
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

// *** TEXTBOX ELEMENT ***
void ele_textbox::draw(Window* win)
{
	std::vector<std::string> broken = break_into_lines(*text, sizex);

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
	(*text) = load_to_delim(datastream, STD_DELIM);
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
	std::vector<std::string> broken = break_into_lines(*text, sizex);
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
	return break_into_lines(*text, sizex);
}