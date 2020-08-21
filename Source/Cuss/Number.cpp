// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Cuss/Number.hpp"
#include <Cataclysm/Screen/Debugger.hpp>

using namespace cuss;

#define SELECTCOLOR c_blue

void ele_number::draw(Doryen::Console& _console)
{
	nc_color hilite = (selected ? SELECTCOLOR : bg);
	if (align == ALIGN_RIGHT)
	{
		_console.write(posx, posy, std::to_string(*value));
	}
	else if (align == ALIGN_CENTER)
	{
		_console.write(posx, posy, std::to_string(*value));
	}
	else
	{
		_console.write(posx, posy, std::to_string(*value));
	}
}

// *** NUMBER ELEMENT ***
void ele_number::draw(Window* win)
{
	nc_color hilite = (selected ? SELECTCOLOR : bg);
	if (align == ALIGN_RIGHT)
	{
		win->putstr_r(posx, posy, fg, hilite, sizex, "%d", (*value));
	}
	else if (align == ALIGN_CENTER)
	{
		win->putstr_c(posx, posy, fg, hilite, sizex, "%d", (*value));
	}
	else
	{
		win->putstr_n(posx, posy, fg, hilite, sizex, "%d", (*value));
	}
}

std::string ele_number::save_data()
{
	std::stringstream ret;
	ret << element::save_data() << " " << (*value);
	return ret.str();
}

void ele_number::load_data(std::istream& datastream)
{
	element::load_data(datastream);
	datastream >> (*value);
}

bool ele_number::self_reference()
{
	if (owns_data)
		return false;

	value = new int;
	(*value) = 0;
	owns_data = true;
	return true;
}

bool ele_number::set_data(int data)
{
	(*value) = data;
	return true;
}

bool ele_number::add_data(int data)
{
	(*value) += data;
	return true;
}

bool ele_number::ref_data(int* data)
{
	if (owns_data)
		delete value;

	value = data;
	owns_data = false;
	return true;
}

bool ele_number::handle_keypress(long ch)
{
	if (ch >= '0' && ch <= '9')
	{
		if (recently_selected)
		{
			(*value) = (ch - '0');
		}
		else
		{
			(*value) *= 10;
			(*value) += (ch - '0');
		}
		return true;
	}
	else if (is_backspace(ch))
	{
		(*value) /= 10;
		return true;
	}
	else if (ch == '-')
	{
		(*value) *= -1;
	}
	return false;
}
