// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Cuss/Binding.hpp"
#include <Cataclysm/Util/String/stringfunc.h>

using namespace cuss;


std::string binding::save_data()
{
	std::stringstream ret;
	ret << int(act) << " " << target << " " STD_DELIM << " " << a << " " << b;
	return ret.str();
}

void binding::load_data(std::istream& datastream)
{
	int tmpact;
	datastream >> tmpact;
	target = load_to_delim(datastream, STD_DELIM);
	datastream >> a >> b;
	act = action_id(tmpact);
}