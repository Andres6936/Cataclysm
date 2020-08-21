// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include <Cataclysm/Screen/Debugger.hpp>
#include "Cataclysm/Item/TypeLauncher.hpp"

Item_type_launcher::Item_type_launcher()
{
	damage = 0;
	recoil = 0;
	durability = 100;
	capacity = 15;
	reload_ap = 300;
	fire_ap = 100;
}

std::string Item_type_launcher::get_property_description()
{
	std::stringstream ret;
	ret << "<c=white>Ammo:<c=/> " << ammo_type << std::endl;
	ret << "<c=ltred>Damage bonus:<c=/> " << (damage >= 0 ? "+" : "") <<
		damage << std::endl;
	ret << "<c=cyan>Accuracy:<c=/> " << accuracy.str() << std::endl;
	ret << "<c=magenta>Recoil:<c=/> " << (recoil >= 0 ? "+" : "") <<
		recoil << std::endl;
	ret << "<c=brown>Durability:<c=/> " << durability << std::endl;
	ret << "<c=ltblue>Capacity:<c=/> " << capacity << " rounds" << std::endl;
	ret << "<c=green>Reload time:<c=/> " << reload_ap / 100 << "." <<
		(reload_ap % 100) / 10 << reload_ap % 10 << " turns" << std::endl;
	ret << "<c=ltgreen>Fire time:<c=/> " << fire_ap / 100 << "." <<
		(fire_ap % 100) / 10 << fire_ap % 10 << " turns" << std::endl;
	ret << "<c=red>Fire modes:<c=/> ";
// TODO: If there's special mods, like "snipe," handle them specially
	if (modes.empty())
	{
		ret << "Single shot";
	}
	for (int i = 0; i < modes.size(); i++)
	{
		ret << "[" << modes[i] << "] ";
	}
	return ret.str();
}


bool Item_type_launcher::handle_data(std::string ident, std::istream& data)
{
	std::string junk;
	if (ident == "type:" || ident == "ammo_type:")
	{
		std::getline(data, ammo_type);

	}
	else if (ident == "skill:")
	{
		std::string skill_name;
		std::getline(data, skill_name);
		skill_used = lookup_skill_type(skill_name);
		if (skill_used == SKILL_NULL)
		{
			debugmsg("No such skill as '%s' (%s)", skill_name.c_str(), name.c_str());
			return false;
		}

	}
	else if (ident == "damage:")
	{
		data >> damage;
		std::getline(data, junk);

	}
	else if (ident == "accuracy:")
	{
		if (!accuracy.load_data(data, name))
		{
			return false;
		}

	}
	else if (ident == "recoil:")
	{
		data >> recoil;
		std::getline(data, junk);

	}
	else if (ident == "durability:")
	{
		data >> durability;
		std::getline(data, junk);

// "Clip" and "magazine" to satisfy the gun nerds.  what up gun nerds
	}
	else if (ident == "clip:" || ident == "magazine:" || ident == "capacity:")
	{
		data >> capacity;
		std::getline(data, junk);

	}
	else if (ident == "reload_time:" || ident == "reload_ap:")
	{
		data >> reload_ap;
		std::getline(data, junk);

	}
	else if (ident == "fire_time:" || ident == "fire_ap:")
	{
		data >> fire_ap;
		std::getline(data, junk);

	}
	else if (ident == "modes:" || ident == "mode:")
	{
		std::string mode_line;
		std::getline(data, mode_line);
		std::istringstream mode_ss(mode_line);
		int mode;
		while (mode_ss >> mode)
		{
			modes.push_back(mode);
		}

	}
	else if (ident != "done")
	{
		return false;
	}
	return true;
}
