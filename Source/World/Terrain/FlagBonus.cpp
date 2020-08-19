// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Terrain/FlagBonus.hpp"
#include "Cataclysm/window.h"

// Terrain_flag_bonus loads from a single line, so it uses a different paradigm
bool Terrain_flag_bonus::load_data(std::istream& data, std::string owner_name)
{
// Get flag name
	if (!data.good())
	{
		debugmsg("Terrain_flag_bonus wasn't fed data (%s)", owner_name.c_str());
		return false;
	}
	std::string flag_name;
	data >> flag_name;
	flag = lookup_terrain_flag(flag_name);

	if (flag == TF_NULL)
	{
		debugmsg("Unknown Terrain_flag name '%s' (%s)",
				flag_name.c_str(), owner_name.c_str());
		return false;
	}

// Get amount
	if (!data.good())
	{
		debugmsg("Terrain_flag_bonus couldn't read amount (%s)",
				owner_name.c_str());
		return false;
	}
	data >> amount;

// Success!
	return true;
}

