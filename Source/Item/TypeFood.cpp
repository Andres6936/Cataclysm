// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Item/TypeFood.hpp"


Item_type_food::Item_type_food()
{
	food = 0;
	water = 0;
	charges = 1;
	verb = "eat";
	effect_chance = 100;
}

std::string Item_type_food::get_property_description()
{
	std::stringstream ret;
	ret << "<c=green>Nutrition: <c=/>" << food << std::endl;
	ret << "<c=ltblue>Water: <c=/>" << water << std::endl;
	return ret.str();
}


bool Item_type_food::handle_data(std::string ident, std::istream& data)
{
	std::string junk;

	if (ident == "food:")
	{
		data >> food;
		std::getline(data, junk);

	}
	else if (ident == "water:")
	{
		data >> water;
		std::getline(data, junk);

	}
	else if (ident == "effect_chance:")
	{
		data >> effect_chance;
		std::getline(data, junk);
		if (effect_chance <= 0)
		{
			debugmsg("Food effect_chance of %d corrected to 1 (%s)",
					effect_chance, name.c_str());
			effect_chance = 1;
		}
		else if (effect_chance > 100)
		{
			debugmsg("Food effect_chance of %d corrected to 100 (%s)",
					effect_chance, name.c_str());
			effect_chance = 100;
		}

	}
	else if (ident == "effect:")
	{
		if (!effect.load_data(data, name))
		{
			return false;
		}

	}
	else if (ident == "charges:")
	{
		data >> charges;
		std::getline(data, junk);

	}
	else if (ident == "verb:")
	{
		std::getline(data, verb);
		verb = no_caps(verb);
		verb = trim(verb);
		if (verb.empty())
		{
			debugmsg("Empty verb (%s)", name.c_str());
			return false;
		}

	}
	else if (ident != "done")
	{
		return false;
	}
	return true;
}