#include <sstream>

#include "Cataclysm/World/world_terrain.h"
#include <Cataclysm/Util/String/String.hpp>
#include <Cataclysm/Util/globals.h>

World_terrain::World_terrain()
{
	uid = -1;
	name = "ERROR";
	beach_range = -1;
	road_cost = 100;
	spread_cost = 1;
	spread_type = SPREAD_NORMAL;
	sym = glyph();
	for (int i = 0; i < WTF_MAX; i++)
	{
		flags.push_back(false);
	}
}

std::string World_terrain::get_data_name()
{
	return name;
}

std::string World_terrain::get_name()
{
	if (display_name.empty())
	{
		return name;
	}
	return display_name;
}

bool World_terrain::load_data(std::istream& data)
{
	std::string ident, junk;
	do
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
		else if (ident == "beach:")
		{
			std::getline(data, beach_name);
			beach_name = trim(beach_name);

		}
		else if (ident == "beach_range:")
		{
			data >> beach_range;
			std::getline(data, junk);

		}
		else if (ident == "connector:")
		{
			std::string conn;
			std::getline(data, conn);
			conn = trim(conn);
			connectors.push_back(conn);

		}
		else if (ident == "road_cost:")
		{
			data >> road_cost;
			std::getline(data, junk);

		}
		else if (ident == "spread_cost:")
		{
			data >> spread_cost;
			std::getline(data, junk);

		}
		else if (ident == "spread:")
		{
			if (!spread.load_data(data, name))
			{
				return false;
			}

		}
		else if (ident == "spread_type:")
		{
			std::string spread_name;
			std::getline(data, spread_name);
			spread_type = lookup_spread_type(spread_name);
			if (spread_type == SPREAD_NULL)
			{
				debugmsg("Unknown Spread_type '%s' (%s)",
						spread_name.c_str(), name.c_str());
				return false;
			}

		}
		else if (ident == "spread_options:")
		{
			std::getline(data, spread_options);
			spread_options = trim(spread_options);
/*
      std::istringstream spread_data(spread_line);
      if (!spread_options.load_data(spread_data, name)) {
        debugmsg("Error loading spread_options for '%s'", name.c_str());
        return false;
      }
*/

		}
		else if (ident == "glyph:")
		{
			sym.load_data_text(data);
			std::getline(data, junk);

		}
		else if (ident == "flags:")
		{
			std::string flag_line;
			std::getline(data, flag_line);
			std::istringstream flag_data(flag_line);
			std::string flag_name;
			while (flag_data >> flag_name)
			{
				World_terrain_flag flag = lookup_world_terrain_flag(flag_name);
				if (flag == WTF_NULL)
				{
					debugmsg("Unknown world terrain flag '%s' (%s)", flag_name.c_str(),
							name.c_str());
				}
				flags[flag] = true;
			}

		}
		else if (ident != "done")
		{
			debugmsg("Unknown World_terrain property '%s' (%s)",
					ident.c_str(), name.c_str());
		}
	} while (ident != "done" && !data.eof());

// TODO: Flag loading.
	return true;
}

bool World_terrain::has_flag(World_terrain_flag flag)
{
	return flags[flag];
}

World_terrain* make_into_beach(World_terrain* original)
{
	if (original->beach_name.empty())
	{
		return original;
	}
	return WORLD_TERRAIN.lookup_name(original->beach_name);
}

