// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/globals.h>
#include <Cataclysm/Entity/Monster/monster.h>
#include "Cataclysm/Entity/Monster/AbilitySignal.hpp"

Monster_ability_signal::Monster_ability_signal()
{
	range = 1;
}



bool Monster_ability_signal::handle_data(std::string ident, std::istream& data,
		std::string owner)
{
	std::string junk;

	if (ident == "signal:")
	{
		std::string line;
		std::getline(data, line);
		std::istringstream signal_data(line);
		if (!signal.load_data(signal_data, owner + " Monster_ability_signal"))
		{
			return false;
		}

	}
	else if (ident == "range:")
	{
		data >> range;
		std::getline(data, junk);

	}
	else
	{
		debugmsg("Unknown Monster_ability_signal property '%s' (%s).",
				ident.c_str(), owner.c_str());
		return false;
	}
	return true;
}

bool Monster_ability_signal::effect(Monster* user)
{
	if (!user)
	{
		debugmsg("Monster_ability_signal::effect(NULL) called!");
		return false;
	}

	if (signal.empty())
	{
		debugmsg("%s tried to use Monster_ability_signal but no signals listed.",
				user->get_name().c_str());
		return false;
	}

	if (range < 0)
	{
		debugmsg("%s tried to use Monster_ability_signal but range was %d!",
				user->get_name().c_str(), range);
		return false;
	}

	Tripoint pos;
	std::string sig = signal.pick();
	for (pos.x = user->pos.x - range; pos.x <= user->pos.x + range; pos.x++)
	{
		for (pos.y = user->pos.y - range; pos.y <= user->pos.y + range; pos.y++)
		{
			pos.z = user->pos.z;
			GAME.map->apply_signal(sig, pos, user);
		}
	}
	return true;
}
