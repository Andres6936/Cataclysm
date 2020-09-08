// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ELEMENT_HPP
#define CATACLYSM_ELEMENT_HPP

#include <string>
#include <vector>
#include <Doryen/Doryen.hpp>
#include <Cuss/Enum/Alignment.hpp>
#include <Cuss/Enum/ElementType.hpp>
#include <Cataclysm/Graphics/color.h>
#include <Cataclysm/Graphics/window.h>
#include <Cuss/Enum/VerticalAligment.hpp>

namespace cuss
{

	struct element
	{
		std::string name;
		int posx;
		int posy;
		int sizex;
		int sizey;
		bool selected;
		bool selectable;
		bool owns_data;
		nc_color fg;
		nc_color bg;
		alignment align;
		vertical_alignment v_align;

		bool recently_selected;

		element()
		{
			name = "";
			posx = 0;
			posy = 0;
			sizex = 0;
			sizey = 0;
			selected = false;
			selectable = false;
			fg = c_ltgray;
			bg = c_black;
			owns_data = true;
			recently_selected = false;
		}

		virtual ~element()
		{
		}

		virtual element_type type()
		{
			return ELE_NULL;
		}

		virtual void draw(std::shared_ptr<Window> win)
		{
		}

		virtual void draw(Doryen::Console& _console)
		{

		};

		virtual std::string save_data();

		virtual void load_data(std::istream& datastream);

		virtual bool self_reference()
		{
			return false;
		}

		virtual bool set_data(std::string data)
		{
			return false;
		}

		virtual bool add_data(std::string data)
		{
			return false;
		}

		virtual bool ref_data(std::string* data)
		{
			return false;
		}

		virtual bool set_data(std::vector<std::string> data)
		{
			return false;
		}

		virtual bool add_data(std::vector<std::string> data)
		{
			return false;
		}

		virtual bool ref_data(std::vector<std::string>* data)
		{
			return false;
		}

		virtual bool set_data(int data)
		{
			return false;
		}

		virtual bool add_data(int data)
		{
			return false;
		}

		virtual bool ref_data(int* data)
		{
			return false;
		}

		virtual bool set_data(glyph gl, int posx, int posy)
		{
			return false;
		}

		virtual bool handle_keypress(long ch)
		{
			return false;
		}

// This is used to set fg & bg, and hence is defined for element!
		virtual bool set_data(nc_color FG, nc_color BG = c_null);

		virtual bool set_alignment(alignment al)
		{
			align = al;
			return true;
		}

		virtual void clear_data()
		{
		}

		virtual std::string get_str()
		{
			std::string ret;
			return ret;
		}

		virtual int get_int()
		{
			return 0;
		}

		virtual std::vector<std::string> get_str_list()
		{
			std::vector<std::string> ret;
			return ret;
		}
	};

}

#endif //CATACLYSM_ELEMENT_HPP
