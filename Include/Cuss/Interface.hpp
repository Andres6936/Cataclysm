#ifndef _CUSS_H_
#define _CUSS_H_

#include <map>
#include <string>
#include <vector>
#include "Cuss/Element.hpp"
#include "Cuss/Binding.hpp"
#include <Cuss/Graphics/window.h>
#include <Cuss/Enum/ElementType.hpp>
#include <Cuss/Enum/ActionID.hpp>

namespace cuss
{

	class interface
	{
	public:
		interface(std::string N = "", int X = 80, int Y = 24);

		~interface();

		void add_element(element_type type, std::string name, int posx, int posy,
				int sizex, int sizey, bool selectable = true);

		bool erase_element(std::string name);

		void draw(std::shared_ptr<Window> win);

		void draw(Doryen::Console& _console);

		void draw_prototype(std::shared_ptr<Window> win); // For the editor

		std::string save_data();

		void load_data(std::istream& datastream);

		bool save_to_file(std::string filename);

		bool load_from_file(std::string filename, bool warn = true);

		std::vector<std::string> element_names();

		element* selected();

		element* find_by_name(std::string name);

		element* select_next(bool force = false);

		element* select_last(bool force = false);

		element* select(std::string name);

		void select_none();

		bool move_element_up(std::string name);

		bool move_element_down(std::string name);

		bool set_selectable(std::string name, bool setting);

// set_data replaces the element's data with whatever is passed
// add_data appends whatever is passed to the element's data
// These are all defined for each element type; if they're invalid, the type
//  just returns false.
		bool set_data(std::string name, std::string data);

		bool add_data(std::string name, std::string data);

		bool set_data(std::string name, std::vector<std::string> data);

		bool add_data(std::string name, std::vector<std::string> data);

		bool set_data(std::string name, int data);

		bool add_data(std::string name, int data);

		bool set_data(std::string name, glyph gl, int posx, int posy);

		bool set_data(std::string name, nc_color fg, nc_color bg = c_null);

/* self_reference makes an element take control of its own data (meaning it will
 * delete the data when it is deleted).
 * ref_data makes an element reference some external set of data.  Note that if
 * a function is not appropriate, it will not change the reference and will
 * return false (e.g. telling a number element to reference a vector of strings)
 */
		bool self_reference(std::string name);

		bool ref_data(std::string name, std::string* data);

		bool ref_data(std::string name, std::vector<std::string>* data);

		bool ref_data(std::string name, int* data);

		bool clear_data(std::string name);

// These will return empty data if inappropriate to the element.
		std::string get_str(std::string name);

		int get_int(std::string name);

		std::vector<std::string> get_str_list(std::string name);

		int element_height(std::string name);

		int element_width(std::string name);

		int element_posx(std::string name);

		int element_posy(std::string name);

		Point element_pos(std::string name);

		std::vector<std::string> binding_list();

		bool add_binding(long ch, action_id act, std::string target = "");

		bool add_binding(long ch, action_id act, std::string target,
				int a, int b = 0);

		binding* bound_to(long ch);

		bool has_bindings_for(action_id act);

		bool has_bindings_for(std::string target);

		bool rem_binding(long ch);

		bool rem_all_bindings(action_id act = ACT_NULL);

		bool rem_all_bindings(std::string target);

		bool set_use_bindings(bool set = true);

		bool handle_action(long ch);  // Only does keybindings
		bool handle_keypress(long ch);  // May redirect to current object
		// e.g. 0-9 will be used as input for number

		std::string name;
		int sizex, sizey;

	private:
		int active_element;
		std::vector<element*> elements;
		std::map<long, binding> bindings;
		bool use_bindings;
	};

}; // namespace cuss

#endif
