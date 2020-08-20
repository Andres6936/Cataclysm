#ifndef _WINDOW_H_
#define _WINDOW_H_

#if (defined _WIN32 || defined WINDOWS)
#include "catacurse.h"
#else

#include <curses.h>
#include "Cataclysm/glyph.h"

#endif

#define KEY_ESC 27

#include <list>

enum Window_type
{
	WINDOW_TYPE_GENERAL,
	WINDOW_TYPE_GLYPHS,
	WINDOW_TYPE_TEXT,
	WINDOW_TYPE_OTHER
};

class Window
{
public:
	Window();

	Window(int posx, int posy, int sizex, int sizey,
			Window_type ntype = WINDOW_TYPE_GENERAL);

	~Window();

	void init(int posx, int posy, int sizex, int sizey,
			Window_type ntype = WINDOW_TYPE_GENERAL);

	void close();

	void resize(int sizex, int sizey);

	void outline();

// Info functions
	glyph glyphat(int x, int y);

	int sizex()
	{
		return xdim;
	}

	int sizey()
	{
		return ydim;
	}

// The three essential output functions
	void putch(int x, int y, nc_color fg, nc_color bg, long sym);

	void putglyph(int x, int y, glyph gl);

// Putstr places a string (unless we're designated as tiles-only)
	void putstr(int x, int y, nc_color fg, nc_color bg, std::string str, ...);

// Putstr_raw ignores color tags
	void putstr_raw(int x, int y, nc_color fg, nc_color bg, std::string str, ...);

// Putstr_n limits the length to maxlength
	void putstr_n(int x, int y, nc_color fg, nc_color bg, int maxlength,
			std::string str, ...);

// Putstr_r is like putstr_n, but right-aligned
	void putstr_r(int x, int y, nc_color fg, nc_color bg, int maxlength,
			std::string str, ...);

// Putstr_c is like putstr_n, but center-aligned
	void putstr_c(int x, int y, nc_color fg, nc_color bg, int maxlength,
			std::string str, ...);

// Special helper drawing functions
	void clear_area(int x1, int y1, int x2, int y2);

	void line_v(int x, nc_color fg = c_white, nc_color bg = c_black);

	void line_h(int y, nc_color fg = c_white, nc_color bg = c_black);

	void clear();

	void refresh();

private:
	WINDOW* w;
	bool outlined;
	Window_type type;
	int xdim, ydim;
};

extern std::list<Window*> WINDOWLIST;

#endif
