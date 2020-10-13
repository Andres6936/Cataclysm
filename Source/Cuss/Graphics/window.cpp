#include <list>

#include <Cuss/Graphics/window.h>
#include <Cataclysm/Screen/Debugger.hpp>

bool parse_color_tags(std::string text, std::vector<std::string>& segments,
		std::vector<long>& color_pairs, nc_color fg = c_white,
		nc_color bg = c_black);

std::string strip_tags(std::string text);

std::list<Window*> WINDOWLIST;

Window::Window()
{
	w = newwin(0, 0, 0, 0);
	outlined = false;
	xdim = 0;
	ydim = 0;
	type = WINDOW_TYPE_GENERAL;
	WINDOWLIST.push_back(this);
}

Window::Window(int posx, int posy, int sizex, int sizey, Window_type ntype)
{
	w = newwin(sizey, sizex, posy, posx);
	outlined = false;
	xdim = sizex;
	ydim = sizey;
	type = ntype;
	WINDOWLIST.push_back(this);
}

Window::~Window()
{
	delwin(w);
	//WINDOWLIST.remove(this);
}

void Window::init(int posx, int posy, int sizex, int sizey, Window_type ntype)
{
	delwin(w);
	w = newwin(sizey, sizex, posy, posx);
	xdim = sizex;
	ydim = sizey;
	type = ntype;
}

void Window::close()
{
	delwin(w);
	WINDOWLIST.remove(this);
	refresh_all(true);
}

void Window::resize(int sizex, int sizey)
{
	wresize(w, sizey, sizex);
}

void Window::outline()
{
	outlined = true;
	long col = get_color_pair(c_white, c_black);
	wattron(w, col);
	wborder(w, LINE_XOXO, LINE_XOXO, LINE_OXOX, LINE_OXOX,
			LINE_OXXO, LINE_OOXX, LINE_XXOO, LINE_XOOX);
	wattroff(w, col);
}

glyph Window::glyphat(int x, int y)
{
	glyph ret;
	if (type == WINDOW_TYPE_TEXT || type == WINDOW_TYPE_OTHER ||
		x < 0 || x >= xdim || y < 0 || y >= ydim)
		return ret; // Whatever a default glyph is

	long wi = mvwinch(w, y, x);
	//ret.symbol = wi - ((wi & A_COLOR) + (wi & A_ATTRIBUTES));
	ret.symbol = wi & A_CHARTEXT;
	extract_colors(wi & A_COLOR, wi & A_ATTRIBUTES, ret.fg, ret.bg);
	return ret;
}

void Window::putch(int x, int y, nc_color fg, nc_color bg, long sym)
{
/*
 if (outlined) {
  x++;
  y++;
 }
*/
	long col = get_color_pair(fg, bg);
	wattron(w, col);
	mvwaddch(w, y, x, sym);
	wattroff(w, col);
}

void Window::putglyph(int x, int y, glyph gl)
{
	if (type == WINDOW_TYPE_TEXT || type == WINDOW_TYPE_OTHER)
		return;
	putch(x, y, gl.fg, gl.bg, gl.symbol);
}

void Window::putstr(int x, int y, nc_color fg, nc_color bg, std::string str,
		...)
{
	if (type == WINDOW_TYPE_GLYPHS)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	std::string prepped = buff;
	long col = get_color_pair(fg, bg);

	if (prepped.find("<c=") == std::string::npos)
	{
// No need to do color segments, so just print!
		wattron(w, col);
		mvwprintw(w, y, x, buff);
		wattroff(w, col);
	}
	else
	{ // We need to do color segments!
		wmove(w, y, x);
		std::vector<std::string> segments;
		std::vector<long> color_pairs;
		parse_color_tags(prepped, segments, color_pairs, fg, bg);
		for (int i = 0; i < segments.size(); i++)
		{
			wattron(w, color_pairs[i]);
			wprintw(w, segments[i].c_str());
			wattroff(w, color_pairs[i]);
		}
	}        // We need to do color segments!

}

void Window::putstr_raw(int x, int y, nc_color fg, nc_color bg, std::string str,
		...)
{
	if (type == WINDOW_TYPE_GLYPHS)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	std::string prepped = buff;
	long col = get_color_pair(fg, bg);

	wattron(w, col);
	mvwprintw(w, y, x, buff);
	wattroff(w, col);
}

void Window::putstr_n(int x, int y, nc_color fg, nc_color bg, int maxlength,
		std::string str, ...)
{
	if (type == WINDOW_TYPE_GLYPHS)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	std::string prepped = buff;
	long col = get_color_pair(fg, bg);

	if (prepped.find("<c=") == std::string::npos)
	{
// No need to do color segments, so just print!
		wattron(w, col);
		mvwprintw(w, y, x, prepped.substr(0, maxlength).c_str());
		wattroff(w, col);
	}
	else
	{ // We need to do color segments!
		wmove(w, y, x);
		std::vector<std::string> segments;
		std::vector<long> color_pairs;
		parse_color_tags(prepped, segments, color_pairs, fg, bg);
		for (int i = 0; i < segments.size(); i++)
		{
			wattron(w, color_pairs[i]);
			if (segments[i].length() > maxlength)
			{
				wprintw(w, segments[i].substr(0, maxlength).c_str());
				wattroff(w, color_pairs[i]);
				return; // Stop; we've run out of space.
			}
			else
			{
				wprintw(w, segments[i].c_str());
				maxlength -= segments[i].length();
				wattroff(w, color_pairs[i]);
			}
		}
	} // We need to do color segments!

}

void Window::putstr_r(int x, int y, nc_color fg, nc_color bg, int maxlength,
		std::string str, ...)
{
	if (type == WINDOW_TYPE_GLYPHS)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	std::string prepped = buff;
	std::string tagless = strip_tags(prepped);
	//std::string tagless = prepped;
	if (tagless.length() < maxlength)
	{
		x += maxlength - tagless.length();
	}
	long col = get_color_pair(fg, bg);

	if (prepped.find("<c=") == std::string::npos)
	{
// No need to do color segments, so just print!
		wattron(w, col);
		mvwprintw(w, y, x, prepped.substr(0, maxlength).c_str());
		wattroff(w, col);
	}
	else
	{ // We need to do color segments!
		wmove(w, y, x);
		std::vector<std::string> segments;
		std::vector<long> color_pairs;
		parse_color_tags(prepped, segments, color_pairs, fg, bg);
		for (int i = 0; i < segments.size(); i++)
		{
			wattron(w, color_pairs[i]);
			if (segments[i].length() > maxlength)
			{
				wprintw(w, segments[i].substr(0, maxlength).c_str());
				wattroff(w, color_pairs[i]);
				return; // Stop; we've run out of space.
			}
			else
			{
				wprintw(w, segments[i].c_str());
				maxlength -= segments[i].length();
				wattroff(w, color_pairs[i]);
			}
		}
	} // We need to do color segments!
}

void Window::putstr_c(int x, int y, nc_color fg, nc_color bg, int maxlength,
		std::string str, ...)
{
	if (type == WINDOW_TYPE_GLYPHS)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	std::string prepped = buff;
	std::string tagless = strip_tags(prepped);
	//std::string tagless = prepped;
	if (tagless.length() < maxlength)
	{
		x += (maxlength - tagless.length()) / 2;
	}
	long col = get_color_pair(fg, bg);

	if (prepped.find("<c=") == std::string::npos)
	{
// No need to do color segments, so just print!
		wattron(w, col);
		mvwprintw(w, y, x, prepped.substr(0, maxlength).c_str());
		wattroff(w, col);
	}
	else
	{ // We need to do color segments!
		wmove(w, y, x);
		std::vector<std::string> segments;
		std::vector<long> color_pairs;
		parse_color_tags(prepped, segments, color_pairs, fg, bg);
		for (int i = 0; i < segments.size(); i++)
		{
			wattron(w, color_pairs[i]);
			if (segments[i].length() > maxlength)
			{
				wprintw(w, segments[i].substr(0, maxlength).c_str());
				wattroff(w, color_pairs[i]);
				return; // Stop; we've run out of space.
			}
			else
			{
				wprintw(w, segments[i].c_str());
				maxlength -= segments[i].length();
				wattroff(w, color_pairs[i]);
			}
		}
	} // We need to do color segments!
}

void Window::clear_area(int x1, int y1, int x2, int y2)
{
	if (type == WINDOW_TYPE_OTHER)
		return;
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
			putch(x, y, c_black, c_black, 'x');
	}
}

void Window::line_v(int x, nc_color fg, nc_color bg)
{
	if (type != WINDOW_TYPE_GENERAL) // Only general (read: non-graphic) windows!
		return;
	for (int y = (outlined ? 1 : 0); y < (outlined ? ydim - 1 : ydim); y++)
		putch(x, y, fg, bg, LINE_XOXO);

	if (outlined)
	{ // Alter the outline so it attaches to our line
		putch(x, 0, fg, bg, LINE_OXXX);
		putch(x, ydim - 1, fg, bg, LINE_XXOX);
	}
}

void Window::line_h(int y, nc_color fg, nc_color bg)
{
	if (type != WINDOW_TYPE_GENERAL) // Only general (read: non-graphic) windows!
		return;
	for (int x = (outlined ? 1 : 0); x < (outlined ? xdim - 1 : xdim); x++)
		putch(x, y, fg, bg, LINE_OXOX);

	if (outlined)
	{ // Alter the outline so it attaches to our line
		putch(0, y, fg, bg, LINE_XXXO);
		putch(xdim - 1, y, fg, bg, LINE_XOXX);
	}
}

void Window::clear()
{
	werase(w);
}

void Window::refresh()
{
	wrefresh(w);
}
