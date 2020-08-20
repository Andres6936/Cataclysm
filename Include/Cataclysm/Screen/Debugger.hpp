// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_DEBUGGER_HPP
#define CATACLYSM_DEBUGGER_HPP

#include <string>
#include <vector>

//      LINE_NESW  - X for on, O for off
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

void init_display();

void end_display();

long input();

void debugmsg(const char* mes, ...);

void refresh_all(bool erase = false);

void get_screen_dims(int& xdim, int& ydim);

std::string key_name(long ch);

bool is_backspace(long ch);

std::string file_selector(std::string start = ".");

std::string string_edit_popup(std::string orig, const char* mes, ...);

std::string string_input_popup(const char* mes, ...);

int int_input_popup(const char* mes, ...);

long popup_getkey(const char* mes, ...);

bool query_yn(const char* mes, ...);

int menu_vec(const char* mes, std::vector<std::string> options);

int menu(const char* mes, ...);

void popup(const char* mes, ...);

void popup_nowait(const char* mes, ...);

void popup_fullscreen(const char* mes, ...);

void popup_scrollable(const char* mes, ...);

#endif //CATACLYSM_DEBUGGER_HPP
