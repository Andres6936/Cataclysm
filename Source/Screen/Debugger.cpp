// Joan Andrés (@Andres6936) Github.

#include <list>
#include <cstring>
#include <curses.h>

#include <Cataclysm/globals.h>
#include "Cataclysm/Screen/Debugger.hpp"

void init_display()
{
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, true);
	init_colors();
	curs_set(0);
	timeout(1);
	getch();
	timeout(-1);
}

void end_display()
{
	erase();
	endwin();
}

long input()
{
	flushinp();
	return getch();
}

void debugmsg(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[2048];
	vsprintf(buff, mes, ap);
	va_end(ap);
	attron(COLOR_PAIR(3));
	mvprintw(0, 0, "DEBUG: %s      \n  Press spacebar...", buff);
	while (getch() != ' ');
	attroff(COLOR_PAIR(3));
}

void refresh_all(bool erase) // erase defaults to false
{
	if (erase)
		clear();

	for (std::list<Window*>::iterator it = WINDOWLIST.begin();
		 it != WINDOWLIST.end(); it++)
		(*it)->refresh();
}

void get_screen_dims(int& xdim, int& ydim)
{
	getmaxyx(stdscr, ydim, xdim);
}

std::string key_name(long ch)
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

bool is_backspace(long ch)
{
	return (ch == KEY_BACKSPACE || ch == 127 || ch == 8);
}

/*
std::string file_selector(std::string start)
{
 #if (defined _WIN32 || defined __WIN32__)
  debugmsg("Sorry, file_selector() not yet coded for Windows.");
  return;
 #endif
 int winx, winy;
 getmaxyx(stdscr, winx, winy); // Get window size

 Window w_select(0, 0, winx, winy);
 w_select.outline();

*/

std::string string_input_popup(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[1024];
	vsprintf(buff, mes, ap);
	va_end(ap);
	return string_edit_popup("", buff);
}

std::string string_edit_popup(std::string orig, const char* mes, ...)
{
	std::string ret = orig;
	va_list ap;
	va_start(ap, mes);
	char buff[1024];
	vsprintf(buff, mes, ap);
	va_end(ap);
	int startx = std::strlen(buff) + 2;
	Window w(0, 11, 80, 3);
	w.outline();
	w.putstr(1, 1, c_ltred, c_black, buff);
	w.putstr(startx, 1, c_magenta, c_black, ret);
	for (int i = startx + ret.length() + 1; i < 79; i++)
		w.putch(i, 1, c_ltgray, c_black, '_');
	int posx = startx + ret.length();
	w.putch(posx, 1, c_ltgray, c_blue, '_');
	do
	{
		w.refresh();
		long ch = getch();
		if (ch == 27)
		{    // Escape
			return orig;
		}
		else if (ch == '\n')
		{
			return ret;
		}
		else if (ch == KEY_BACKSPACE || ch == 127)
		{
			if (posx > startx)
			{
// Move the cursor back and re-draw it
				ret = ret.substr(0, ret.size() - 1);
				w.putch(posx, 1, c_ltgray, c_black, '_');
				posx--;
				w.putch(posx, 1, c_ltgray, c_blue, '_');
			}
		}
		else
		{
			ret += ch;
			w.putch(posx, 1, c_magenta, c_black, ch);
			posx++;
			w.putch(posx, 1, c_ltgray, c_blue, '_');
		}
	} while (true);
}

int int_input_popup(const char* mes, ...)
{
	std::string ret;
	bool negative = false;
	va_list ap;
	va_start(ap, mes);
	char buff[1024];
	vsprintf(buff, mes, ap);
	va_end(ap);
	int startx = strlen(buff) + 3;
	Window w(0, 11, 80, 3);
	w.outline();
	w.putstr(1, 1, c_ltred, c_black, buff);
	w.putstr(startx, 1, c_magenta, c_black, ret);
	for (int i = startx + ret.length() + 1; i < 79; i++)
		w.putch(i, 1, c_ltgray, c_black, '_');
	int posx = startx + ret.length();
	w.putch(posx, 1, c_ltgray, c_blue, '_');
	bool done = false;
	while (!done)
	{
		w.refresh();
		long ch = getch();
		if (ch == 27)
		{    // Escape
			return 0;
		}
		else if (ch == '\n')
		{
			done = true;
		}
		else if (ch == KEY_BACKSPACE || ch == 127)
		{
			if (posx > startx)
			{
// Move the cursor back and re-draw it
				ret = ret.substr(0, ret.size() - 1);
				w.putch(posx, 1, c_ltgray, c_black, '_');
				posx--;
				w.putch(posx, 1, c_ltgray, c_blue, '_');
			}
		}
		else if (ch >= '0' && ch <= '9')
		{
			ret += ch;
			w.putch(posx, 1, c_magenta, c_black, ch);
			posx++;
			w.putch(posx, 1, c_ltgray, c_blue, '_');
		}
		else if (ch == '-')
		{
			negative = !negative;
			if (negative)
				w.putch(startx - 1, 1, c_magenta, c_black, '-');
			else
				w.putch(startx - 1, 1, c_black, c_black, 'x');
		}
	}

	int retnum = 0;
	for (int i = 0; i < ret.length(); i++)
	{
		int val = ret[i] - '0';
		for (int n = 0; n < (ret.length() - 1 - i); n++)
			val *= 10;
		retnum += val;
	}
	if (negative)
		retnum *= -1;
	return retnum;
}

long popup_getkey(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 0;
	int height = 2;
	size_t pos = tmp.find('\n');
	while (pos != std::string::npos)
	{
		height++;
		if (pos > width)
			width = pos;
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	if (width == 0 || tmp.length() > width)
		width = tmp.length();
	width += 2;
	if (height > 25)
		height = 25;
	Window w(int((80 - width) / 2), int((25 - height) / 2), width, height + 1);
	w.outline();
	tmp = buff;
	pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_white, c_black, line);
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_white, c_black, std::string(tmp));

	w.refresh();
	long ch = getch();
	return ch;
}

bool query_yn(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 0;
	int height = 2;
	size_t pos = tmp.find('\n');
	while (pos != std::string::npos)
	{
		height++;
		if (pos > width)
			width = pos;
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	if (width == 0 || tmp.length() > width)
		width = tmp.length();
	width += 2;
	if (height > 25)
		height = 25;
	Window w(int((80 - width) / 2), int((25 - height) / 2), width, height + 1);
	w.outline();
	tmp = buff;
	pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_ltgray, c_black, line);
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_ltgray, c_black, std::string(tmp));

	w.refresh();
	long ch = getch();
	return (ch == 'y' || ch == 'Y');
}

int menu_vec(const char* mes, std::vector<std::string> options)
{
	if (options.size() == 0)
	{
		debugmsg("0-length menu (\"%s\")", mes);
		return -1;
	}
	std::string title = mes;
	int height = 3 + options.size(), width = title.length() + 2;
	for (int i = 0; i < options.size(); i++)
	{
		if (options[i].length() + 6 > width)
			width = options[i].length() + 6;
	}
	Window w(10, 6, width, height);
	w.outline();
	w.putstr(1, 1, c_white, c_black, title);

	for (int i = 0; i < options.size(); i++)
		w.putstr(1, i + 2, c_white, c_black, "%c: %s", 'a' + i, options[i].c_str());
	long ch;
	w.refresh();
	do
		ch = getch();
	while (ch < 'a' || ch >= 'a' + options.size());
	return (ch - 'a');
}

int menu(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char* tmp;
	std::vector<std::string> options;
	bool done = false;
	while (!done)
	{
		tmp = va_arg(ap, char*);
		if (tmp != NULL)
		{
			std::string strtmp = tmp;
			options.push_back(strtmp);
		}
		else
			done = true;
	}
	return (menu_vec(mes, options));
}

void popup(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 0;
	int height = 2;
	size_t pos = tmp.find('\n');
	while (pos != std::string::npos)
	{
		height++;
		if (pos > width)
			width = pos;
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	if (width == 0 || tmp.length() > width)
		width = tmp.length();
	width += 2;
	if (height > 25)
		height = 25;
	Window w(int((80 - width) / 2), int((25 - height) / 2), width, height + 1);
	w.outline();
	tmp = buff;
	pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_ltgray, c_black, line.c_str());
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_ltgray, c_black, tmp.c_str());

	w.refresh();
	long ch;
	do
		ch = getch();
	while (ch != ' ' && ch != '\n' && ch != KEY_ESC);
}

void popup_nowait(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 0;
	int height = 2;
	size_t pos = tmp.find('\n');
	while (pos != std::string::npos)
	{
		height++;
		if (pos > width)
			width = pos;
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	if (width == 0 || tmp.length() > width)
		width = tmp.length();
	width += 2;
	if (height > 25)
		height = 25;
	Window w(int((80 - width) / 2), int((25 - height) / 2), width, height + 1);
	w.outline();
	tmp = buff;
	pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_ltgray, c_black, line.c_str());
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_ltgray, c_black, tmp.c_str());

	w.refresh();
}

void popup_fullscreen(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 80;
	int height = 24;
	Window w(0, 0, width, height);
	tmp = buff;
	int pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_ltgray, c_black, line.c_str());
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_ltgray, c_black, tmp.c_str());

	w.refresh();
	long ch;
	do
		ch = getch();
	while (ch != ' ' && ch != '\n' && ch != KEY_ESC);
}

void popup_scrollable(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[65536];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 80;
	int height = 24;
	std::shared_ptr<Window> w = std::make_shared<Window>(0, 0, width, height);
	cuss::interface i_popup;
	i_popup.add_element(cuss::ELE_TEXTBOX, "text", 0, 0, 80, 24, false);
	i_popup.set_data("text", tmp);
	i_popup.set_data("text", c_ltgray);
/*
  int pos = tmp.find('\n');
  int line_num = 0;
  while (pos != std::string::npos) {
    std::string line = tmp.substr(0, pos);
    line_num++;
    w.putstr(1, line_num, c_ltgray, c_black, line.c_str());
    tmp = tmp.substr(pos + 1);
    pos = tmp.find('\n');
  }
  line_num++;
  w.putstr(1, line_num, c_ltgray, c_black, tmp.c_str());
*/

	long ch;
	do
	{
		i_popup.draw(w);
		ch = getch();
		if (ch == 'k' || ch == 'K' || ch == KEY_UP)
		{
			i_popup.add_data("text", -1);
		}
		else if (ch == 'j' || ch == 'J' || ch == KEY_DOWN)
		{
			i_popup.add_data("text", 1);
		}
		else if (ch == ' ')
		{
			i_popup.add_data("text", 10);
		}
		else if (ch == 'b' || ch == 'B')
		{
			i_popup.add_data("text", -10);
		}
	} while (ch != '\n' && ch != KEY_ESC);
}

bool parse_color_tags(std::string text, std::vector<std::string>& segments,
		std::vector<long>& color_pairs, nc_color fg, nc_color bg)
{
	size_t tag;
	nc_color cur_fg = fg, cur_bg = bg;

	while ((tag = text.find("<c=")) != std::string::npos)
	{
// Everything before the tag is a segment, with the current colors
		segments.push_back(text.substr(0, tag));
		color_pairs.push_back(get_color_pair(cur_fg, cur_bg));
// Strip off everything up to and including "<c="
		text = text.substr(tag + 3);
// Find the end of the tag
		size_t tagend = text.find(">");
		if (tagend == std::string::npos)
		{
			debugmsg("Unterminated color tag! %d:%s:",
					int(tag), text.c_str());
			return false;
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
					debugmsg("Malformed color tag: %s", tag.c_str());
					return false;
				}
			}
			else
			{
				nc_color new_fg = color_string(tag.substr(0, comma)),
						new_bg = color_string(tag.substr(comma + 1));
				if (new_fg == c_null && new_bg == c_null)
				{
					debugmsg("Malformed color tag: %s", tag.c_str());
					return false;
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

	if (segments.size() != color_pairs.size())
	{
		debugmsg("Segments.size() = %d, color_pairs.size() = %d",
				segments.size(), color_pairs.size());
		return false;
	}

	return true;
}

std::string strip_tags(std::string text)
{
	size_t tag;
	std::string ret;
	while ((tag = text.find("<c=")) != std::string::npos)
	{
		ret += text.substr(0, tag);
// Strip off everything up to and including "<c="
		text = text.substr(tag + 3);
// Find the end of the tag
		size_t tagend = text.find(">");
		if (tagend == std::string::npos)
		{
// Couldn't find the end of the tag, so just return
			ret += text;
			return ret;
		}
		text = text.substr(tagend + 1);
	}
	ret += text;
	return ret;
}


