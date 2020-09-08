// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Terrain/Smash.hpp"
#include <Cataclysm/Screen/Debugger.hpp>
#include <Cataclysm/Util/String/stringfunc.h>

Terrain_smash::Terrain_smash()
{
	armor[DAMAGE_PIERCE] = Dice(0, 0, 50); // Ignore pierce by default.
	ignore_chance = 0;
}

bool Terrain_smash::load_data(std::istream& data, std::string name)
{
	std::string ident, junk;
	while (ident != "done" && !data.eof())
	{
		if (!(data >> ident))
		{
			debugmsg("Failed to read smash data (%s)", name.c_str());
			return false;
		}
		ident = no_caps(ident);

		if (!ident.empty() && ident[0] == '#')
		{
// It's a comment
			std::getline(data, junk);

		}
		else if (ident == "success_sound:")
		{
			std::getline(data, success_sound);
			success_sound = trim(success_sound);

		}
		else if (ident == "failure_sound:")
		{
			std::getline(data, failure_sound);
			failure_sound = trim(failure_sound);

		}
		else if (ident == "armor:")
		{
			std::string damage_name;
			data >> damage_name;
			Damage_type dmgtype = lookup_damage_type(damage_name);
			if (dmgtype == DAMAGE_NULL)
			{
				debugmsg("Invalid armor type '%s' in smash data for '%s'",
						damage_name.c_str(), name.c_str());
				return false;
			}
			else
			{
				armor[dmgtype].load_data(data, name);
			}

		}
		else if (ident == "ignore_chance:")
		{
			data >> ignore_chance;

		}
		else if (ident != "done")
		{
			debugmsg("Unknown Terrain_smash flag '%s' (%s)",
					ident.c_str(), name.c_str());
			return false;
		}
	}
	return true;
}

