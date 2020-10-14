// Joan Andrés (@Andres6936) Github.

#include "Cuss/List.hpp"
#include <Cataclysm/Util/String.hpp>
#include <Cuss/String/Utility.hpp>
#include <Cataclysm/Screen/Debugger.hpp>
#include <sstream>

using namespace cuss;

#define SELECTCOLOR c_blue


void _print_scrollbar(std::shared_ptr<Window> win, int posx, int posy, int length, int offset,
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

void ele_list::draw(Doryen::Console& _console)
{
	for (int i = 0; i + offset < list->size() && i < sizey; i++)
	{
		nc_color hilite = (selection == i + offset ? SELECTCOLOR : bg);
		int ypos, index;
		if (v_align == ALIGN_BOTTOM)
		{
			ypos = posy + sizey - 1 - i;
			index = list->size() - 1 - i - offset;
		}
		else
		{ // Default to top-aligned
			ypos = posy + i;
			index = i + offset;
		}
		if (!selected)
			hilite = bg;

		std::vector<std::string> segments;
		std::vector<long> color_pairs;

		const auto colors = Utility::parseColorTags((*list)[index], segments, color_pairs, fg, hilite);

		std::uint32_t positionX = align == ALIGN_RIGHT ? posx : posx + 4;
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

	//if (selectable) _print_scrollbar(win, posx + sizex - 1, posy, sizey, offset, list->size(), selected);
}

// *** LIST ELEMENT ***
void ele_list::draw(std::shared_ptr<Window> win)
{
	win->clear_area(posx, posy, posx + sizex - 1, posy + sizey - 1);

	for (int i = 0; i + offset < list->size() && i < sizey; i++)
	{
		nc_color hilite = (selection == i + offset ? SELECTCOLOR : bg);
		int ypos, index;
		if (v_align == ALIGN_BOTTOM)
		{
			ypos = posy + sizey - 1 - i;
			index = list->size() - 1 - i - offset;
		}
		else
		{ // Default to top-aligned
			ypos = posy + i;
			index = i + offset;
		}
		if (!selected)
			hilite = bg;
		if (align == ALIGN_RIGHT)
		{
/* If it's selectable, we need an extra space at the end to compensate for the
 * scroll bar; otherwise the scroll bar will cover up the last name in the list.
 * This is hacky but it's good enough for now.
 */
			if (selectable)
			{
				win->putstr_r(posx, ypos, fg, hilite, sizex, (*list)[index] + " ");
			}
			else
			{
				win->putstr_r(posx, ypos, fg, hilite, sizex, (*list)[index]);
			}
		}
		else if (align == ALIGN_CENTER)
		{
			win->putstr_c(posx, ypos, fg, hilite, sizex, (*list)[index]);
		}
		else
		{
			win->putstr_n(posx, ypos, fg, hilite, sizex, (*list)[index]);
		}
	}

	if (selectable)
		_print_scrollbar(win, posx + sizex - 1, posy, sizey, offset, list->size(),
				selected);
}

std::string ele_list::save_data()
{
	std::stringstream ret;
	ret << element::save_data() << " " << list->size() << " ";
	for (int i = 0; i < list->size(); i++)
		ret << (*list)[i] << " " << STD_DELIM << " ";

	return ret.str();
}

void ele_list::load_data(std::istream& datastream)
{
	element::load_data(datastream);
	int tmpsize;
	datastream >> tmpsize;
	for (int i = 0; i < tmpsize; i++)
	{
		std::string tmp = Utility::load_to_delim(datastream, STD_DELIM);
		list->push_back(tmp);
	}
}

bool ele_list::self_reference()
{
	if (owns_data)
		return false;

	list = new std::vector<std::string>;
	owns_data = true;
	return true;
}

bool ele_list::set_data(std::string data)
{
	list->clear();
	add_data(data);
	return true;
}

bool ele_list::add_data(std::string data)
{
	list->push_back(data);
	return true;
}

bool ele_list::set_data(std::vector<std::string> data)
{
	(*list) = data;
	selection = 0;
	offset = 0;
	return true;
}

bool ele_list::add_data(std::vector<std::string> data)
{
	for (int i = 0; i < data.size(); i++)
		list->push_back(data[i]);
	return true;
}

bool ele_list::ref_data(std::vector<std::string>* data)
{
	if (owns_data)
		delete list;

	list = data;
	owns_data = false;
	return true;
}

bool ele_list::set_data(int data)
{
	selection = data;

	if (selection < 0)
		selection = 0;
	if (selection >= list->size())
		selection = list->size() - 1;

	if (selection < sizey)
		offset = 0;
	else if (selection >= list->size() - sizey)
		offset = list->size() - sizey;
	else
		offset = selection;

	return true;
}

bool ele_list::add_data(int data)
{
	selection += data;
	if (selection < 0)
		selection = 0;
	if (selection >= list->size())
		selection = list->size() - 1;

	while (selection < offset)
		offset--;
	if (offset + sizey <= selection)
		offset = selection - sizey + 1;

	return true;
}

int ele_list::get_int()
{
	return selection;
}

std::string ele_list::get_str()
{
	if (selection < 0 || selection >= list->size())
	{
		std::string ret;
		return ret;
	}

	return (*list)[selection];
}
