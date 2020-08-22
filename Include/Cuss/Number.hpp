// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_NUMBER_HPP
#define CATACLYSM_NUMBER_HPP

#include <string>
#include "Element.hpp"

namespace cuss
{

	struct ele_number : public element
	{
		int* value;

		ele_number()
		{
			name = "";
			posx = 0;
			posy = 0;
			sizex = 0;
			sizey = 0;
			selected = false;
			selectable = false;
			value = 0;
			fg = c_ltgray;
			bg = c_black;
			owns_data = false;
			self_reference();
		}

		virtual element_type type()
		{
			return ELE_NUMBER;
		};

		virtual void draw(std::shared_ptr<Window> win);

		void draw(Doryen::Console& _console) override;

		virtual std::string save_data();

		virtual void load_data(std::istream& datastream);

		virtual bool self_reference();

		virtual bool set_data(int data);

		virtual bool add_data(int data);

		virtual bool ref_data(int* data);

		virtual bool handle_keypress(long ch);

		virtual void clear_data()
		{
			(*value) = 0;
		};

		virtual int get_int()
		{
			return (*value);
		};
	};

}

#endif //CATACLYSM_NUMBER_HPP
