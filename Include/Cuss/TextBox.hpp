// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TEXTBOX_HPP
#define CATACLYSM_TEXTBOX_HPP

#include <string>
#include <vector>
#include "Element.hpp"

namespace cuss
{

	struct ele_textbox : public element
	{
		std::string* text;
		int offset;

		ele_textbox()
		{
			name = "";
			posx = 0;
			posy = 0;
			sizex = 0;
			sizey = 0;
			selected = false;
			selectable = false;
			offset = 0;
			fg = c_ltgray;
			bg = c_black;
			owns_data = false;
			self_reference();
		}

		~ele_textbox()
		{
			if (owns_data) delete text;
		};

		virtual element_type type()
		{
			return ELE_TEXTBOX;
		};

		virtual void draw(Window* win);

		virtual std::string save_data();

		virtual void load_data(std::istream& datastream);

		/**
		 * We store this as a vector because the text needs to be split into seperate
		 * lines.  It's more efficient to do this once, when the text is stored, than
		 * every time we print.
		 */
		virtual bool self_reference();

		virtual bool set_data(std::string data);

		virtual bool add_data(std::string data);

		virtual bool ref_data(std::string* data);

		virtual bool set_data(std::vector<std::string> data);

		virtual bool add_data(std::vector<std::string> data);

		// These adjust the offset
		virtual bool set_data(int data);

		virtual bool add_data(int data);

		virtual bool handle_keypress(long ch)
		{
			return false;
		};

		virtual void clear_data()
		{
			(*text) = "";
			offset = 0;
		};

		virtual std::string get_str()
		{
			return (*text);
		}

		virtual std::vector<std::string> get_str_list();
	};

}

#endif //CATACLYSM_TEXTBOX_HPP
