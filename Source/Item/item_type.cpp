#include <Cataclysm/Util/String/String.hpp>
#include <Cataclysm/Item/item_type.h>
#include <Cataclysm/Screen/Debugger.hpp>
#include <sstream>

Item_type::Item_type()
{
	uid = -1;
	name = "bug";
	sym = glyph();
	weight = 0;
	volume = 0;
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		damage[i] = 0;
	}
	to_hit = 0;
	attack_speed = 0;
	thrown_variance = Dice(8, 20, 0);
	thrown_dmg_percent = 50;
	thrown_speed = 0;
	for (int i = 0; i < ITEM_FLAG_MAX; i++)
	{
		flags.push_back(false);
	}
	mission_experience = 0;
}

Item_type::~Item_type()
{
}




void Item_type::assign_uid(int id)
{
	uid = id;
}

std::string Item_type::get_data_name()
{
	return name;
}

std::string Item_type::get_name()
{
	if (display_name.empty())
	{
		return name;
	}
	return display_name;
}

// This is overloaded for Item_type classes that need more (like Item_type_ammo)
std::string Item_type::get_name_singular()
{
	return get_name();
}

std::string Item_type::get_name_plural()
{
	if (has_flag(ITEM_FLAG_PLURAL))
	{
		return get_name_singular();
	}
	else if (plural_name.empty())
	{
		return get_name_singular() + "s";
	}
	return plural_name;
}

bool Item_type::load_data(std::istream& data)
{
	std::string ident, junk;
	bool set_name = false, set_glyph = false;
	while (ident != "done" && !data.eof())
	{
		if (!(data >> ident))
		{
			debugmsg("Couldn't read Item_type data (%s)",
					(set_name ? name.c_str() : "Name not set!"));
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
			set_name = true;

		}
		else if (ident == "display_name:")
		{
			std::getline(data, display_name);
			display_name = trim(display_name);

		}
		else if (ident == "plural:")
		{
			std::getline(data, plural_name);
			plural_name = trim(plural_name);

		}
		else if (ident == "description:")
		{
			std::string desc;
			description = "";
			while (no_caps(desc) != "done")
			{
				std::getline(data, desc);
				desc = trim(desc);
				if (no_caps(desc) != "done")
				{
					description = description + " " + desc;
				}
			}
			description = trim(description);  // Get rid of extra " "

		}
		else if (ident == "glyph:")
		{
			sym.load_data_text(data, name);
			std::getline(data, junk);
			set_glyph = true;

		}
		else if (ident == "weight:")
		{
			data >> weight;
			std::getline(data, junk);

		}
		else if (ident == "volume:")
		{
			data >> volume;
			std::getline(data, junk);

		}
		else if (ident == "bash:")
		{
			data >> damage[DAMAGE_BASH];
			std::getline(data, junk);

		}
		else if (ident == "cut:")
		{
			data >> damage[DAMAGE_CUT];
			std::getline(data, junk);

		}
		else if (ident == "pierce:")
		{
			data >> damage[DAMAGE_PIERCE];
			std::getline(data, junk);

		}
		else if (ident == "to_hit:")
		{
			data >> to_hit;
			std::getline(data, junk);

		}
		else if (ident == "speed:" || ident == "attack_speed:")
		{
			data >> attack_speed;
			std::getline(data, junk);

		}
		else if (ident == "thrown_variance:")
		{
			if (!thrown_variance.load_data(data, name))
			{
				return false;
			}

		}
		else if (ident == "thrown_dmg_percent:")
		{
			data >> thrown_dmg_percent;
			std::getline(data, junk);

		}
		else if (ident == "thrown_speed:")
		{
			data >> thrown_speed;
			std::getline(data, junk);

		}
		else if (ident == "mission_xp:")
		{
			data >> mission_experience;
			std::getline(data, junk);

		}
		else if (ident == "container:")
		{
			std::getline(data, container);
			container = no_caps(container);
			container = trim(container);
			if (container.empty())
			{
				debugmsg("Empty container (%s)", name.c_str());
				return false;
			}

		}
		else if (ident == "flags:")
		{
			std::string flag_line;
			std::getline(data, flag_line);
			std::istringstream flag_data(flag_line);
			std::string flag_name;
			while (flag_data >> flag_name)
			{
				Item_flag flag = lookup_item_flag(flag_name);
				if (flag == ITEM_FLAG_NULL)
				{
					debugmsg("Unknown item flag '%s' (%s)",
							flag_name.c_str(), name.c_str());
					return false;
				}
				flags[flag] = true;
			}

		}
		else if (ident != "done" && !handle_data(ident, data))
		{
			debugmsg("Unknown Item_type flag '%s' (%s)", ident.c_str(), name.c_str());
			return false;

		}
	}
// Ensure that we set a glyph and name!
	if (!set_name)
	{
		debugmsg("Item created without a name!");
		return false;
	}
	if (!set_glyph)
	{
		debugmsg("Item '%s' created without a glyph!", name.c_str());
		return false;
	}
	return true;
}

bool Item_type::handle_data(std::string ident, std::istream& data)
{
	if (ident == "done")
	{
		return true;
	}
	return false;
}

bool Item_type::has_flag(Item_flag flag)
{
	return flags[flag];
}
