// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_MENU_HPP
#define CATACLYSM_MENU_HPP

#include <string>
#include <vector>
#include "Element.hpp"

namespace cuss
{

	struct ele_menu : public element
	{
		std::string title;
		std::vector<std::string>* list;
		int selection;
		bool open;

		ele_menu()
		{
			name = "";
			posx = 0;
			posy = 0;
			sizex = 0;
			sizey = 0;
			selected = false;
			selectable = false;
			selection = -1;
			open = false;
			fg = c_ltgray;
			bg = c_black;
			title = "";
			owns_data = false;
			self_reference();
		}

		virtual element_type type()
		{
			return ELE_MENU;
		};

		virtual void draw(Window* win);

		virtual std::string save_data();

		virtual void load_data(std::istream& datastream);

		virtual bool self_reference();

// set_data sets the title--the only string unique identified w/o an index
		virtual bool set_data(std::string data);

// add_data adds an option to the menu--the only place where adding makes sense
		virtual bool add_data(std::string data);

		virtual bool set_data(std::vector<std::string> data);

		virtual bool add_data(std::vector<std::string> data);

		virtual bool ref_data(std::vector<std::string>* data);

// Change the selection
		virtual bool set_data(int data);

		virtual bool add_data(int data);

		virtual bool handle_keypress(long ch);

		virtual void clear_data()
		{
			title.clear();
			list->clear();
			open = false;
			selection = 0;
		};

		virtual std::string get_str();

		virtual int get_int();

		virtual std::vector<std::string> get_str_list()
		{
			return (*list);
		};
	};

}

#endif //CATACLYSM_MENU_HPP
