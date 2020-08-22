#include <Cuss/Drawing.hpp>
#include <Cuss/List.hpp>
#include <Cuss/TextBox.hpp>
#include <Cuss/Menu.hpp>
#include <Cuss/TextEntry.hpp>
#include "Cuss/Interface.hpp"
#include <Cataclysm/Screen/Debugger.hpp>
#include <Cataclysm/stringfunc.h>
#include <Cuss/Number.hpp>
#include <sstream>
#include <fstream>

using namespace cuss;

#define PREP_ELEMENT(ele) \
  (ele)->name = name;\
  (ele)->posx = posx;\
  (ele)->posy = posy;\
  (ele)->sizex = szx;\
  (ele)->sizey = szy;\
  (ele)->selected = false;\
  (ele)->selectable = selectable;\
  (ele)->align = ALIGN_LEFT

std::string cuss::element_type_name(cuss::element_type type)
{
	switch (type)
	{
	case ELE_NULL:
		return "NULL";
	case ELE_DRAWING:
		return "Drawing";
	case ELE_TEXTBOX:
		return "Text";
	case ELE_LIST:
		return "List selection";
	case ELE_TEXTENTRY:
		return "Text Entry";
	case ELE_NUMBER:
		return "Number";
	case ELE_MENU:
		return "Menu";
	default:
		return "Unknown";
	}
	return "What the heck";
}

interface::interface(std::string N, int X, int Y)
{
	active_element = -1;
	use_bindings = false;
	name = N;
	sizex = X;
	sizey = Y;
}

interface::~interface()
{
	for (int i = 0; i < elements.size(); i++)
		delete elements[i];
}

void interface::add_element(element_type type, std::string name, int posx,
		int posy, int szx, int szy, bool selectable)
{
	if (posx < 0 || posx >= sizex || posy < 0 || posy >= sizey)
		return;

	if (posx + szx >= sizex)
		szx = sizex - posx;
	if (posy + szy >= sizey)
		szy = sizey - posy;

	if (name.find(' ') != std::string::npos)
		return;

	switch (type)
	{

	case ELE_NULL:
		return; // We don't have any reason to actually add these, right?

	case ELE_DRAWING:
	{
		ele_drawing* ele = new ele_drawing;
		PREP_ELEMENT(ele);
		elements.push_back(ele);
	}
		break;

	case ELE_TEXTBOX:
	{
		ele_textbox* ele = new ele_textbox;
		PREP_ELEMENT(ele);
		elements.push_back(ele);
	}
		break;

	case ELE_LIST:
	{
		ele_list* ele = new ele_list;
		PREP_ELEMENT(ele);
		elements.push_back(ele);
	}
		break;

	case ELE_TEXTENTRY:
	{
		ele_textentry* ele = new ele_textentry;
		PREP_ELEMENT(ele);
		elements.push_back(ele);
	}
		break;

	case ELE_NUMBER:
	{
		ele_number* ele = new ele_number;
		PREP_ELEMENT(ele);
		elements.push_back(ele);
	}
		break;

	case ELE_MENU:
	{
		ele_menu* ele = new ele_menu;
		PREP_ELEMENT(ele);
		elements.push_back(ele);
	}
		break;

	default:
		debugmsg("Unknown element type %d", type);
		return;
	}

	if (active_element == -1)
		active_element = 0;
}

bool interface::erase_element(std::string name)
{
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->name == name)
		{
			element* el = elements[i];
			elements.erase(elements.begin() + i);
			delete el;
			if (active_element == i)
				active_element = -1;
			return true;
		}
	}
	return false;
}

void interface::draw(Doryen::Console& _console)
{
	_console.clear();
	std::vector<element*> draw_last; // Menus need to be layered at the top
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->type() == ELE_MENU && elements[i]->selected)
			draw_last.push_back(elements[i]);
		else
			elements[i]->draw(_console);
	}

	for (int i = 0; i < draw_last.size(); i++)
		draw_last[i]->draw(_console);

	_console.draw();
}


void interface::draw(std::shared_ptr<Window> win)
{
	win->clear();
	std::vector<element*> draw_last; // Menus need to be layered at the top
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->type() == ELE_MENU && elements[i]->selected)
			draw_last.push_back(elements[i]);
		else
			elements[i]->draw(win);
	}

	for (int i = 0; i < draw_last.size(); i++)
		draw_last[i]->draw(win);

	win->refresh();
}

void interface::draw_prototype(std::shared_ptr<Window> win)
{
	win->clear();
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->name != "BG")
		{
			int x1 = elements[i]->posx, y1 = elements[i]->posy;
			int x2 = x1 + elements[i]->sizex - 1, y2 = y1 + elements[i]->sizey - 1;
// Draw the background color
			for (int x = x1; x <= x2; x++)
			{
				for (int y = y1; y <= y2; y++)
				{
					element_type type = elements[i]->type();
					win->putch(x, y, c_black,
							type == ELE_DRAWING ? c_dkgray : nc_color(2 + type), ' ');
				}
			}
// Draw the corner/side delineators
			if (y1 == y2)
			{
				win->putch(x1, y1, c_white, c_black, LINE_XXXO);
				win->putch(x2, y2, c_white, c_black, LINE_XOXX);
			}
			else if (x1 == x2)
			{
				win->putch(x1, y1, c_white, c_black, LINE_OXXX);
				win->putch(x2, y2, c_white, c_black, LINE_XXOX);
			}
			else
			{
				win->putch(x1, y1, c_white, c_black, LINE_OXXO);
				win->putch(x2, y2, c_white, c_black, LINE_XOOX);
			}
			if (elements[i]->align == ALIGN_RIGHT)
			{
				win->putstr_r(x1 + 1, y1, (elements[i]->selected ? c_magenta : c_yellow),
						c_black, elements[i]->sizex - 2, elements[i]->name);
			}
			else if (elements[i]->align == ALIGN_CENTER)
			{
				win->putstr_c(x1 + 1, y1, (elements[i]->selected ? c_magenta : c_yellow),
						c_black, elements[i]->sizex - 2, elements[i]->name);
			}
			else
			{
				win->putstr_n(x1 + 1, y1, (elements[i]->selected ? c_magenta : c_yellow),
						c_black, elements[i]->sizex - 2, elements[i]->name);
			}
		}
		if (elements[i]->type() == ELE_DRAWING)
			elements[i]->draw(win);
	}
}

std::string interface::save_data()
{
	std::stringstream ret;

	ret << name << " " << STD_DELIM << " " << sizex << " " << sizey << " " <<
		elements.size() << " ";
	for (int i = 0; i < elements.size(); i++)
		ret << elements[i]->type() << " " << elements[i]->save_data() << std::endl;

	ret << bindings.size() << " ";
	std::map<long, binding>::iterator it;
	for (it = bindings.begin(); it != bindings.end(); it++)
		ret << it->first << " " << it->second.save_data() << std::endl;

	return ret.str();
}

void interface::load_data(std::istream& datastream)
{
	name = load_to_delim(datastream, STD_DELIM);
	datastream >> sizex >> sizey;
	elements.clear();
	int tmpcount, tmpbind;
	datastream >> tmpcount;
	for (int i = 0; i < tmpcount; i++)
	{
		int tmptype;
		datastream >> tmptype;
		switch (element_type(tmptype))
		{

		case ELE_NULL:
			debugmsg("Loaded NULL element!");
			break;

		case ELE_DRAWING:
		{
			ele_drawing* tmp = new ele_drawing;
			tmp->load_data(datastream);
			elements.push_back(tmp);
		}
			break;

		case ELE_TEXTBOX:
		{
			ele_textbox* tmp = new ele_textbox;
			tmp->load_data(datastream);
			elements.push_back(tmp);
		}
			break;

		case ELE_LIST:
		{
			ele_list* tmp = new ele_list;
			tmp->load_data(datastream);
			elements.push_back(tmp);
		}
			break;

		case ELE_TEXTENTRY:
		{
			ele_textentry* tmp = new ele_textentry;
			tmp->load_data(datastream);
			elements.push_back(tmp);
		}
			break;

		case ELE_NUMBER:
		{
			ele_number* tmp = new ele_number;
			tmp->load_data(datastream);
			elements.push_back(tmp);
		}
			break;

		case ELE_MENU:
		{
			ele_menu* tmp = new ele_menu;
			tmp->load_data(datastream);
			elements.push_back(tmp);
		}
			break;
		}
	} // for (int i = 0; i < tmpcount; i++)

	datastream >> tmpbind;
	for (int i = 0; i < tmpbind; i++)
	{
		long tmpch;
		binding bind;
		datastream >> tmpch;
		bind.load_data(datastream);
		bindings[tmpch] = bind;
	}
}

bool interface::save_to_file(std::string filename)
{
	std::ofstream fout;
	fout.open(filename.c_str());
	if (!fout.is_open())
		return false;

	fout << save_data();
	fout.close();
	return true;
}

bool interface::load_from_file(std::string filename, bool warn)
{
	std::ifstream fin;
	fin.open(filename.c_str());
	if (!fin.is_open())
	{
		if (warn)
		{
			debugmsg("Can't load %s!", filename.c_str());
		}
		return false;
	}

	load_data(fin);
	fin.close();
	return true;
}

std::vector<std::string> interface::element_names()
{
	std::vector<std::string> ret;
	for (int i = 0; i < elements.size(); i++)
		ret.push_back(elements[i]->name);

	return ret;
}

element* interface::selected()
{
	if (active_element < 0 || active_element >= elements.size())
		return NULL;

	return elements[active_element];
}

element* interface::find_by_name(std::string name)
{
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->name == name)
			return elements[i];
	}
	return NULL;
}

element* interface::select_next(bool force)
{
	if (elements.empty())
	{
		active_element = -1;
		return NULL;
	}

	if (active_element >= 0 && active_element < elements.size())
		elements[active_element]->selected = false;

	int tried = 0;
	do
	{
		tried++;
		if (active_element >= elements.size() - 1)
			active_element = 0;
		else
			active_element++;
	} while ((!force && tried < elements.size() &&
			  !elements[active_element]->selectable) ||
			 elements[active_element]->name == "BG");

	if (tried == elements.size() && !elements[active_element]->selectable)
	{
		active_element = -1;
		return NULL;
	}
	else
	{
		elements[active_element]->selected = true;
		elements[active_element]->recently_selected = true;
		return elements[active_element];
	}
	return NULL;
}

element* interface::select_last(bool force)
{
	if (elements.empty())
	{
		active_element = -1;
		return NULL;
	}

	if (active_element >= 0 && active_element < elements.size())
		elements[active_element]->selected = false;

	int tried = 0;
	do
	{
		tried++;
		if (active_element <= 0)
			active_element = elements.size() - 1;
		else
			active_element--;
	} while ((!force && tried < elements.size() &&
			  !elements[active_element]->selectable) ||
			 elements[active_element]->name == "BG");

	if (tried == elements.size() && !elements[active_element]->selectable)
	{
		active_element = -1;
		return NULL;
	}
	else
	{
		elements[active_element]->selected = true;
		elements[active_element]->recently_selected = true;
		return elements[active_element];
	}
	return NULL;
}

element* interface::select(std::string name)
{
	for (int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->name == name)
		{
			if (active_element >= 0 && active_element < elements.size())
				elements[active_element]->selected = false;
			active_element = i;
			elements[active_element]->selected = true;
			elements[active_element]->recently_selected = true;
			return elements[active_element];
		}
	}
	return NULL;
}

void interface::select_none()
{
	if (active_element >= 0 && active_element < elements.size())
		elements[active_element]->selected = false;

	active_element = -1;
}

bool interface::move_element_up(std::string name)
{
// Check all except the last element
	for (int i = 0; i < elements.size() - 1; i++)
	{
		if (elements[i]->name == name)
		{
			element* tmp = elements[i + 1];
			elements[i + 1] = elements[i];
			elements[i] = tmp;
			return true;
		}
	}
	return false;
}

bool interface::move_element_down(std::string name)
{
// Check all except the first element
	for (int i = 1; i < elements.size(); i++)
	{
		if (elements[i]->name == name)
		{
			element* tmp = elements[i - 1];
			elements[i - 1] = elements[i];
			elements[i] = tmp;
			return true;
		}
	}
	return false;
}


bool interface::set_selectable(std::string name, bool setting)
{
	element* el = find_by_name(name);
	if (!el)
		return false;

	el->selectable = setting;
	return true;
}

bool interface::set_data(std::string name, std::string data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->set_data(data);
}

bool interface::add_data(std::string name, std::string data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->add_data(data);
}

bool interface::set_data(std::string name, std::vector<std::string> data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->set_data(data);
}

bool interface::add_data(std::string name, std::vector<std::string> data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->add_data(data);
}

bool interface::set_data(std::string name, int data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->set_data(data);
}

bool interface::add_data(std::string name, int data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->add_data(data);
}

bool interface::set_data(std::string name, glyph gl, int x, int y)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->set_data(gl, x, y);
}

// bg defaults to c_null
bool interface::set_data(std::string name, nc_color fg, nc_color bg)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->set_data(fg, bg);
}

bool interface::self_reference(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->self_reference();
}

bool interface::ref_data(std::string name, std::string* data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->ref_data(data);
}

bool interface::ref_data(std::string name, std::vector<std::string>* data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->ref_data(data);
}

bool interface::ref_data(std::string name, int* data)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	return ele->ref_data(data);
}

bool interface::clear_data(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
		return false;

	ele->clear_data();
	return true;
}

std::string interface::get_str(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
	{
		std::string ret;
		return ret;
	}

	return ele->get_str();
}

int interface::get_int(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
		return 0;

	return ele->get_int();
}

std::vector<std::string> interface::get_str_list(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
	{
		std::vector<std::string> ret;
		return ret;
	}
	return ele->get_str_list();
}

int interface::element_height(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
	{
		return -1;
	}
	return ele->sizey;
}

int interface::element_width(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
	{
		return -1;
	}
	return ele->sizex;
}

int interface::element_posx(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
	{
		return -1;
	}
	return ele->posx;
}

int interface::element_posy(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
	{
		return -1;
	}
	return ele->posy;
}

Point interface::element_pos(std::string name)
{
	element* ele = find_by_name(name);
	if (!ele)
	{
		return Point(-1, -1);
	}
	return Point(ele->posx, ele->posy);
}


std::vector<std::string> interface::binding_list()
{
	std::vector<std::string> ret;

	std::map<long, binding>::iterator it;
	for (it = bindings.begin(); it != bindings.end(); it++)
	{
		std::stringstream info;
		info << key_name(it->first) << ": " << action_name(it->second.act);
		if (it->second.act == ACT_SELECT_STR)
			info << " " << it->second.target;
		else if (it->second.act == ACT_SCROLL)
			info << " (" << it->second.target << (it->second.a >= 0 ? " +" : " ") <<
				 it->second.a << ")";
		else if (it->second.act == ACT_SET_COLORS)
			info << "(" << it->second.target << ", " <<
				 color_name(nc_color(it->second.a)) << ", " <<
				 color_name(nc_color(it->second.b)) << ")";
		else if (it->second.act == ACT_TRANSLATE)
			info << " (" << it->second.target << "; " << char(it->second.a) << " to " <<
				 char(it->second.b) << ")";

		ret.push_back(info.str());
	}
	return ret;
}

bool interface::add_binding(long ch, action_id act, std::string target)
{
	if (bindings.count(ch))
	{
		debugmsg("Binding exists for %d!", ch);
		return false;
	}
	if (action_needs_element(act) && target != "<S>" && !find_by_name(target))
	{
		debugmsg("Couldn't find element \"%s\"!", target.c_str());
		return false;
	}

	binding newbind(act, target);
	bindings[ch] = newbind;
	return true;
}

bool interface::add_binding(long ch, action_id act, std::string target,
		int a, int b)
{
	if (bindings.count(ch))
	{
		debugmsg("Binding exists for %d!", ch);
		return false;
	}
	if (action_needs_element(act) && target != "<S>" && !find_by_name(target))
	{
		debugmsg("Couldn't find element \"%s\"!", target.c_str());
		return false;
	}

	binding newbind(act, target, a, b);
	bindings[ch] = newbind;
	return true;
}

binding* interface::bound_to(long ch)
{
	if (!bindings.count(ch))
		return NULL;

	return &(bindings[ch]);
}

bool interface::has_bindings_for(action_id act)
{
	std::map<long, binding>::iterator it;
	for (it = bindings.begin(); it != bindings.end(); it++)
	{
		if (it->second.act == act)
			return true;
	}
	return false;
}

bool interface::has_bindings_for(std::string target)
{
	std::map<long, binding>::iterator it;
	for (it = bindings.begin(); it != bindings.end(); it++)
	{
		if (it->second.target == target)
			return true;
	}
	return false;
}

bool interface::rem_binding(long ch)
{
	if (!bindings.count(ch))
		return false;

	bindings.erase(ch);
	return true;
}

bool interface::rem_all_bindings(action_id act)
{
	if (bindings.empty())
		return false;

	if (act == ACT_NULL)
	{
		bindings.clear();
		return true;
	}
	std::vector<long> to_delete;
	std::map<long, binding>::iterator it;
	for (it = bindings.begin(); it != bindings.end(); it++)
	{
		if (it->second.act == act)
			to_delete.push_back(it->first);
	}
	if (to_delete.empty())
		return false;

	for (int i = 0; i < to_delete.size(); i++)
		bindings.erase(to_delete[i]);

	return true;
}

bool interface::rem_all_bindings(std::string target)
{
	if (bindings.empty())
		return false;

	std::vector<long> to_delete;
	std::map<long, binding>::iterator it;
	for (it = bindings.begin(); it != bindings.end(); it++)
	{
		if (it->second.target == target)
			to_delete.push_back(it->first);
	}
	if (to_delete.empty())
		return false;

	for (int i = 0; i < to_delete.size(); i++)
		bindings.erase(to_delete[i]);

	return true;
}

bool interface::set_use_bindings(bool set)
{
	use_bindings = set;
	return true;
}

bool interface::handle_action(long ch)
{
	if (!bindings.count(ch))
		return false;

	binding* used = &(bindings[ch]);
	element* found = (used->target == "" || used->target == "<S>" ? selected() :
					  find_by_name(used->target));

	switch (used->act)
	{

	case ACT_NULL:
		return true;

	case ACT_SELECT_NEXT:
		select_next();
		return true;

	case ACT_SELECT_LAST:
		select_last();
		return true;

	case ACT_SELECT_NONE:
		select_none();
		return true;

	case ACT_SELECT_STR:
		if (select(used->target))
			return true;
		return false;

	case ACT_SCROLL:
		if (!found)
			return false;
		if (found->type() != ELE_TEXTBOX && found->type() != ELE_LIST &&
			found->type() != ELE_NUMBER && found->type() != ELE_MENU)
			return false;
		found->add_data(used->a);
		return true;

	case ACT_SET_COLORS:
		if (!found)
			return false;
		found->set_data(nc_color(used->a), nc_color(used->b));
		return true;

	case ACT_TRANSLATE:
		if (!found)
			return false;
		if (found->type() == ELE_DRAWING)
		{
			ele_drawing* draw = static_cast<ele_drawing*>(found);
			draw->translate(used->a, used->b);
			return true;
		}
		else
			return false;

	default:
		return false;
	}

	return false;
}

bool interface::handle_keypress(long ch)
{
	if (handle_action(ch))
	{
		return true; // We had a keybinding for it!
	}
	if (!selected())
	{
		return false;
	}
	else
	{
		bool ret = selected()->handle_keypress(ch);
		selected()->recently_selected = false;
		return ret;
	}
}
