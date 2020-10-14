// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Cuss/Element.hpp"
#include <Cuss/String/Utility.hpp>

using namespace cuss;

// Base save/load functions.
std::string element::save_data()
{
	std::stringstream ret;
	ret << name << " " << STD_DELIM << " " << posx << " " << posy << " " <<
		sizex << " " << sizey << " " << selectable << " " << align << " " <<
		v_align;
	return ret.str();
}

void element::load_data(std::istream& datastream)
{
	name = Utility::load_to_delim(datastream, STD_DELIM);
	int tmpalign, tmpvalign;

	datastream >> posx >> posy >> sizex >> sizey >> selectable >> tmpalign >>
			   tmpvalign;
	align = alignment(tmpalign);
	v_align = vertical_alignment(tmpvalign);
}

bool element::set_data(nc_color FG, nc_color BG)
{
	if (fg != c_null)
		fg = FG;
	if (BG != c_null) // bg defaults to c_null
		bg = BG;

	return true;
}