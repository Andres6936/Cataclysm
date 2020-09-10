#include <Cataclysm/Screen/Debugger.hpp>
#include "Cataclysm/World/Biome/biome.h"
#include <Cataclysm/Util/String/String.hpp>

Biome::Biome()
{
	name = "Unknown";
	uid = -1;
	for (int i = 0; i < BIOME_FLAG_MAX; i++)
	{
		flags.push_back(false);
	}
}

Biome::~Biome()
{
}

void Biome::assign_uid(int id)
{
	uid = id;
}

std::string Biome::get_data_name()
{
	return name;
}

std::string Biome::get_name()
{
	if (display_name.empty())
	{
		return name;
	}
	return display_name;
}

bool Biome::load_data(std::istream& data)
{
	std::string ident, junk;
	while (ident != "done" && !data.eof())
	{

		if (!(data >> ident))
		{
			return false;
		}

		ident = no_caps(ident);

		if (!ident.empty() && ident[0] == '#')
		{
// It's a comment
			std::getline(data, junk);

		}
		else if (ident == "name:")
		{
			std::getline(data, name);
			name = trim(name);

		}
		else if (ident == "display_name:")
		{
			std::getline(data, display_name);
			display_name = trim(display_name);

		}
		else if (ident == "terrain:")
		{
			std::string terrain_line;
			std::getline(data, terrain_line);
			std::istringstream terrain_data(terrain_line);
			if (!terrain.load_data(terrain_data, name))
			{
				debugmsg("Error loading terrain for '%s'", name.c_str());
				return false;
			}

		}
		else if (ident == "bonuses:")
		{
			std::string terrain_line;
			std::getline(data, terrain_line);
			std::istringstream terrain_data(terrain_line);
			if (!bonuses.load_data(terrain_data, name))
			{
				debugmsg("Error loading terrain for '%s'", name.c_str());
				return false;
			}

		}
		else if (ident == "road_bonuses:")
		{
			std::string terrain_line;
			std::getline(data, terrain_line);
			std::istringstream terrain_data(terrain_line);
			if (!road_bonuses.load_data(terrain_data, name))
			{
				debugmsg("Error loading terrain for '%s'", name.c_str());
				return false;
			}

		}
		else if (ident == "monsters:")
		{
			std::string monster_line;
			std::getline(data, monster_line);
			std::istringstream monster_data(monster_line);
			monsters.load_data(monster_data, name);

		}
		else if (ident == "population:" || ident == "monster_population:")
		{
			std::string dice_line;
			std::getline(data, dice_line);
			std::istringstream dice_data(dice_line);
			monster_population.load_data(dice_data, name);

		}
		else if (ident == "flags:")
		{
			std::string line;
			std::getline(data, line);
			std::istringstream flag_data(line);
			std::string flag_name;
			while (flag_data >> flag_name)
			{
				flags[lookup_biome_flag(flag_name)] = true;
			}

		}
		else if (ident != "done")
		{
			debugmsg("Unknown Biome property '%s' (%s)",
					ident.c_str(), name.c_str());
		}
	}
	return true;
}

World_terrain* Biome::pick_terrain()
{
	return terrain.pick();
}

World_terrain* Biome::pick_bonus()
{
	return bonuses.pick();
}

World_terrain* Biome::pick_road_bonus()
{
	return road_bonuses.pick();
}

bool Biome::has_flag(Biome_flag flag)
{
	return flags[flag];
}
