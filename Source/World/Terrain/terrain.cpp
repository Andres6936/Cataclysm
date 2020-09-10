#include "Cataclysm/World/Terrain/terrain.h"
#include <Cataclysm/Util/String.hpp>
#include <Cataclysm/Util/globals.h>

Terrain::Terrain()
{
	uid = -1;
	name = "ERROR";
	movecost = 100;
	height = -1;
	hp = 0;
	smashable = false;
	for (int i = 0; i < TF_MAX; i++)
	{
		flags.push_back(false);
	}
}

std::string Terrain::get_data_name()
{
	return name;
}

std::string Terrain::get_name()
{
	if (display_name.empty())
	{
		return name;
	}
	return display_name;
}

bool Terrain::load_data(std::istream& data)
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
		else if (ident == "inverse:")
		{
			std::getline(data, inverse);
			inverse = trim(inverse);

		}
		else if (ident == "glyph:")
		{
			sym.load_data_text(data, name);
			std::getline(data, junk);

		}
		else if (ident == "movecost:")
		{
			data >> movecost;
			std::getline(data, junk);

		}
		else if (ident == "hp:")
		{
			data >> hp;
			std::getline(data, junk);

		}
		else if (ident == "height:")
		{
			data >> height;
			std::getline(data, junk);

		}
		else if (ident == "smashable" || ident == "smash:")
		{
			std::getline(data, junk);
			if (smash.load_data(data, name))
			{
				smashable = true;
			}
			else
			{
				smash = Terrain_smash();
			}

		}
		else if (ident == "destroy:")
		{
			std::getline(data, destroy_result);
			destroy_result = trim(destroy_result);

		}
		else if (ident == "signal:")
		{
			std::string signal_name;
			std::getline(data, signal_name);
			signal_name = no_caps(signal_name);
			signal_name = trim(signal_name);
			if (signal_handlers.count(signal_name) > 0)
			{
				debugmsg("Defined signal handler for '%s' twice (%s)",
						signal_name.c_str(), name.c_str());
				return false;
			}
			Terrain_signal_handler tmphandler;
			if (!tmphandler.load_data(data, name + "::" + signal_name))
			{
				return false;
			}
			signal_handlers[signal_name] = tmphandler;

		}
		else if (ident == "flags:")
		{
			std::string flag_line;
			std::getline(data, flag_line);
			std::istringstream flagdata(flag_line);
			std::string flagname;
			while (flagdata >> flagname)
			{
				Terrain_flag tf = lookup_terrain_flag(flagname);
				if (tf == TF_NULL)
				{
					debugmsg("Unknown terrain flag '%s' (%s)",
							flagname.c_str(), name.c_str());
					return false;
				}
				flags[tf] = true;
			}

		}
		else if (ident != "done")
		{
			debugmsg("Unknown terrain property '%s' (%s)",
					ident.c_str(), name.c_str());
		}
	}
	if (hp > 0 && destroy_result.empty())
	{
		debugmsg("\
Terrain '%s' has HP %d but no destroy_result.\n\
Either set a destroy_result or omit HP line.",
				name.c_str(), hp);
		return false;
	}

// Finally, set height (if it's not set)
	if (height == -1)
	{
		if (movecost == 0)
		{
			height = 100;
		}
		else
		{
			height = 0;
		}
	}
	return true;
}

bool Terrain::has_flag(Terrain_flag flag)
{
	if (flag < 0 || flag > flags.size())
	{
		debugmsg("Terrain::has_flag error - flag %d/%d, terrain %s",
				flag, flags.size(), name.c_str());
		return false;
	}
	return flags[flag];
}


Terrain_flag lookup_terrain_flag(std::string name)
{
	name = no_caps(name);
	for (int i = 0; i < TF_MAX; i++)
	{
		if (terrain_flag_name(Terrain_flag(i)) == name)
		{
			return Terrain_flag(i);
		}
	}
	return TF_NULL;
}

// Note: ALL terrain flag names must be all lowercase!
std::string terrain_flag_name(Terrain_flag flag)
{
	switch (flag)
	{
	case TF_NULL:
		return "null";
	case TF_OPAQUE:
		return "opaque";
	case TF_FLOOR:
		return "floor";
	case TF_MUTABLE:
		return "mutable";
	case TF_STAIRS_UP:
		return "stairs_up";
	case TF_STAIRS_DOWN:
		return "stairs_down";
	case TF_OPEN_SPACE:
		return "open_space";
	case TF_WATER:
		return "water";
	case TF_FLAMMABLE:
		return "flammable";
	case TF_CONTAINER:
		return "container";
	case TF_PLURAL:
		return "plural";
	case TF_INDOORS:
		return "indoors";
	case TF_SEALED:
		return "sealed";
	case TF_EXPLOSIVE:
		return "explosive";
	case TF_NO_ITEMS:
		return "no_items";
	default:
		return "ERROR"; // All caps means it'll never be used
	}
	return "ERROR";
}
