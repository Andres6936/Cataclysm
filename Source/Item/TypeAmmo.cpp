// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Item/TypeAmmo.hpp"



Item_type_ammo::Item_type_ammo()
{
	damage = 0;
// When we hit a target, their effective armor is (armor * 10) / armor_pierce
// Thus, armor_pierce of 10 means "don't affect their armor at all"
	armor_pierce = 10;
	range = 0;
	count = 100;
	pellets = 1;
}

std::string Item_type_ammo::get_name_singular()
{
	std::stringstream ret;
	ret << "box of " << get_name();
	return ret.str();
}

std::string Item_type_ammo::get_name_plural()
{
	std::stringstream ret;
	ret << "boxes of " << get_name();
	return ret.str();
}

std::string Item_type_ammo::get_property_description()
{
	std::stringstream ret;
	ret << "<c=white>Type:<c=/> " << ammo_type << std::endl;
	ret << "<c=cyan>Accuracy:<c=/> " << accuracy.str() << std::endl;
	if (pellets > 1)
	{
		ret << "<c=ltred>Pellets:<c=/> " << pellets << std::endl;
	}
	ret << "<c=red>Damage:<c=white> " << damage << "\n<c=ltblue>Armor Piercing: ";
	if (armor_pierce == 10)
	{
		ret << "<c=dkgray>None";
	}
	else
	{
		if (armor_pierce < 10)
		{
			ret << "<c=red>";
		}
		else
		{
			ret << "<c=white>";
		}
		ret << "x" << armor_pierce / 10 << "." << armor_pierce % 10;
	}
	ret << "<c=/>\n";
	ret << "<c=ltgray>Range: " << range << "<c=/>\n";

	return ret.str();
}


bool Item_type_ammo::handle_data(std::string ident, std::istream& data)
{
	std::string junk;
	if (ident == "type:")
	{
		std::getline(data, ammo_type);

	}
	else if (ident == "damage:")
	{
		data >> damage;
		std::getline(data, junk);

	}
	else if (ident == "armor_pierce:" || ident == "pierce:")
	{
		data >> armor_pierce;
		if (armor_pierce <= 0)
		{
			armor_pierce = 10;
		}
		std::getline(data, junk);

	}
	else if (ident == "range:")
	{
		data >> range;
		std::getline(data, junk);

	}
	else if (ident == "accuracy:")
	{
		if (!accuracy.load_data(data, name))
		{
			return false;
		}

	}
	else if (ident == "count:")
	{
		data >> count;
		std::getline(data, junk);

	}
	else if (ident == "pellets:")
	{
		data >> pellets;
		std::getline(data, junk);

	}
	else if (ident != "done")
	{
		return false;
	}
	return true;
}
