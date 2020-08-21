// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_LIST_HPP
#define CATACLYSM_LIST_HPP

#include <string>
#include <vector>
#include "Element.hpp"

namespace cuss
{

	struct ele_list : public element
	{
		std::vector<std::string>* list;
		int offset;
		int selection;

		ele_list()
		{
			name = "";
			posx = 0;
			posy = 0;
			sizex = 0;
			sizey = 0;
			selected = false;
			selectable = false;
			offset = 0;
			selection = 0;
			fg = c_ltgray;
			bg = c_black;
			owns_data = false;
			self_reference();
		}

		virtual element_type type()
		{
			return ELE_LIST;
		};

		virtual void draw(Window* win);

		virtual std::string save_data();

		virtual void load_data(std::istream& datastream);

		virtual bool self_reference();

		virtual bool set_data(std::string data);

		virtual bool add_data(std::string data);

		virtual bool set_data(std::vector<std::string> data);

		virtual bool add_data(std::vector<std::string> data);

		virtual bool ref_data(std::vector<std::string>* data);

		// These are used to set the selection
		virtual bool set_data(int data);

		virtual bool add_data(int data);

		// TODO: Implement search function
		virtual bool handle_keypress(long ch)
		{
			return false;
		};

		virtual void clear_data()
		{
			list->clear();
			offset = 0;
			selection = 0;
		};

		virtual int get_int();

		virtual std::string get_str();

		virtual std::vector<std::string> get_str_list()
		{
			return (*list);
		};
	};

}

#endif //CATACLYSM_LIST_HPP
