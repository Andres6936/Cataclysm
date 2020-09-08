// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Entity/Monster/Ability.hpp"
#include "Cataclysm/Screen/Debugger.hpp"
#include <Cataclysm/Util/String/stringfunc.h>

Monster_ability::Monster_ability()
{
	frequency = 0;
	weight = 10;
	AP_cost = 0;
	HP_cost = 0;
}

Monster_ability::~Monster_ability()
{
}


bool Monster_ability::load_data(std::istream& data, std::string owner)
{
	std::string ident, junk;
	while (ident != "done" && !data.eof())
	{
		if (!(data >> ident))
		{
			debugmsg("Couldn't read file for loading Monster_ability (%s)",
					owner.c_str());
			return false;
		}
		ident = no_caps(ident);

		if (!ident.empty() && ident[0] == '#')
		{
// It's a comment; clear the line.
			std::getline(data, junk);

		}
		else if (ident == "freq:" || ident == "frequency:")
		{
			data >> frequency;
			std::getline(data, junk);

		}
		else if (ident == "weight:")
		{
			data >> weight;
			std::getline(data, junk);

		}
		else if (ident == "ap_cost:")
		{
			data >> AP_cost;
			std::getline(data, junk);

		}
		else if (ident == "hp_cost:")
		{
			data >> HP_cost;
			std::getline(data, junk);

		}
		else if (ident == "verb:")
		{
			std::getline(data, verb);
			verb = trim(verb);

		}
		else if (ident != "done" && !handle_data(ident, data, owner))
		{
			debugmsg("Unknown Monster_ability property '%s' (%s)",
					ident.c_str(), owner.c_str());
			return false;
		}
	}
	return true;
}

bool Monster_ability::handle_data(std::string ident, std::istream& data,
		std::string owner)
{
	return false;
}

bool Monster_ability::effect(Monster* user)
{
	debugmsg("Monster_ability::effect() called!  This is a NULL ability!");
	return false;
}