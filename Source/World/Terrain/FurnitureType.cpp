// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/globals.h>
#include <Cataclysm/World/mapgen.h>
#include <Cataclysm/Util/String/stringfunc.h>
#include "Cataclysm/World/Terrain/FurnitureType.hpp"

Furniture_type::Furniture_type()
{
	uid = -1;
	name = "ERROR";
	move_cost = 100;
	height = -1;
	weight = 0;
	hp = 0;
	smashable = false;
	components = new Item_group;
	owns_components = true;
	for (int i = 0; i < TF_MAX; i++)
	{
		flags.push_back(false);
	}
}

Furniture_type::~Furniture_type()
{
	if (owns_components)
	{
		delete components;
	}
}

std::string Furniture_type::get_data_name()
{
	return name;
}

std::string Furniture_type::get_name()
{
	if (display_name.empty())
	{
		return name;
	}
	return display_name;
}

void Furniture_type::assign_uid(int id)
{
	uid = id;
}

bool Furniture_type::has_flag(Terrain_flag flag)
{
	if (flag >= flags.size())
	{
		return false;
	}
	return flags[flag];
}

bool Furniture_type::load_data(std::istream& data)
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
		else if (ident == "glyph:")
		{
			sym.load_data_text(data, name);
			std::getline(data, junk);

		}
		else if (ident == "move_cost:")
		{
			data >> move_cost;
			std::getline(data, junk);

		}
		else if (ident == "height:")
		{
			data >> height;
			std::getline(data, junk);

// This is weight *per tile*
		}
		else if (ident == "weight:")
		{
			data >> weight;
			std::getline(data, junk);

		}
		else if (ident == "hp:")
		{
			data >> hp;
			std::getline(data, junk);

		}
		else if (ident == "smash:")
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
		else if (ident == "components:")
		{
			if (!owns_components)
			{
				debugmsg("Furniture_type tried to load bespoke components after \
loading preset components!");
				return false;
			}
			else if (!components->load_item_data(data, name))
			{
				return false;
			}

		}
		else if (ident == "preset_components:")
		{
			std::string item_group_name;
			std::getline(data, item_group_name);
			item_group_name = trim(item_group_name);
			Item_group* tmpgroup = ITEM_GROUPS.lookup_name(item_group_name);
			if (!tmpgroup)
			{
				debugmsg("Unknown Item_group '%s' (%s)", item_group_name.c_str(),
						name.c_str());
				return false;
			}
// Copy the items
			components = tmpgroup;
// Set owns_components to false, so we won't delete components in our destructor
			owns_components = false;

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
			debugmsg("Unknown Furniture property '%s' (%s)", ident.c_str(),
					name.c_str());
			return false;
		}
	}
	return true;
}
