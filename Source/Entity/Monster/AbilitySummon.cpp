// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Random/rng.h>
#include <Cataclysm/Entity/Pool.hpp>
#include <Cataclysm/Entity/Player/player.h>
#include <Cataclysm/World/ObserverMap.hpp>
#include <Cataclysm/Entity/Monster/monster.h>
#include <Cataclysm/Visual/Screen/MessageQueue.hpp>
#include "Cataclysm/Entity/Monster/AbilitySummon.hpp"

Monster_ability_summon::Monster_ability_summon()
{
	number = Dice(0, 0, 1);
	range = 1;
	max_summons = 0;
	verb = "creates";
}



bool Monster_ability_summon::handle_data(std::string ident, std::istream& data,
		std::string owner)
{
	std::string junk;

	if (ident == "monster:")
	{
		std::string line;
		std::getline(data, line);
		std::istringstream name_data(line);
		if (!monster.load_data(name_data, owner + " Monster_ability_summon"))
		{
			return false;
		}

	}
	else if (ident == "number:")
	{
		if (!number.load_data(data, owner + " Monster_ability_summon"))
		{
			return false;
		}

	}
	else if (ident == "range:")
	{
		data >> range;
		std::getline(data, junk);

	}
	else if (ident == "max_summons:")
	{
		data >> max_summons;
		std::getline(data, junk);

	}
	else
	{
		debugmsg("Unknown Monster_ability_summon property '%s' (%s)",
				ident.c_str(), owner.c_str());
		return false;
	}
	return true;
}

bool Monster_ability_summon::effect(Monster* user)
{
	if (!user)
	{
		debugmsg("Monster_ability_summon::effect(NULL) called!");
		return false;
	}

	if (monster.empty())
	{
		debugmsg("%s tried to use summon ability, but no summons were listed!",
				user->get_data_name().c_str());
		return false; // Unable to pick a monster!
	}

	if (range <= 0)
	{
		debugmsg("%s tried to use summon ability, but range was %d!",
				user->get_data_name().c_str(), range);
		return false; // No range!
	}

	if (user->summons_used >= max_summons)
	{
		return false;
	}

	std::vector<Tripoint> valid_targets;
	Tripoint p;
	for (p.x = user->pos.x - range; p.x <= user->pos.x + range; p.x++)
	{
		for (p.y = user->pos.y - range; p.y <= user->pos.y + range; p.y++)
		{
			p.z = user->pos.z;
			if (p == user->pos)
			{ // Skip central tile
				p.y++;
			}
			if (observerMap.isEmpty(p))
			{
				valid_targets.push_back(p);
			}
		}
	}

	if (valid_targets.empty())
	{
		return false; // No valid spawn points, so cancel the attempt.
	}

	int spawns = number.roll();

	std::map<std::string, int> actual_spawns;

	while (spawns > 0 && user->summons_used < max_summons &&
		   !valid_targets.empty())
	{
		spawns--;
		user->summons_used++;

		std::string mon_name = monster.pick();

		Monster* mon = new Monster(mon_name);
		if (!mon->type)
		{
			debugmsg("%s tried to summon a '%s' but that doesn't exist.",
					user->get_data_name().c_str(), mon_name.c_str());
			delete mon;
			return false;
		}

		int index = rng(0, valid_targets.size() - 1);
		mon->pos = valid_targets[index];
		valid_targets.erase(valid_targets.begin() + index);
		entities.add_entity(mon);

// Track spawn counts, for the monster below.
		if (player->can_sense(mon))
		{
			if (actual_spawns.count(mon_name) == 0)
			{
				actual_spawns[mon_name] = 1;
			}
			else
			{
				actual_spawns[mon_name]++;
			}
		}

	}

// Display a message if the player sees us.
	int nspn = actual_spawns.size();
	bool see_user = player->can_sense(user);
	if (nspn > 0)
	{
		std::stringstream message;
		if (see_user)
		{
			message << user->get_name_to_player() << " " << verb << " ";
		}
		bool need_and = (nspn > 1);
		for (std::map<std::string, int>::iterator it = actual_spawns.begin();
			 it != actual_spawns.end();
			 it++)
		{
			nspn--;
			if (nspn <= 0 && need_and)
			{
				message << " and ";
			}
			if (it->second == 1)
			{
				message << "a " << it->first;
			}
			else
			{
				message << it->second << " " << it->first << "s";
			}
			if (nspn == 1)
			{
				message << " ";
			}
			else if (nspn >= 2)
			{
				message << ", ";
			}
		}
		if (!see_user)
		{
			message << " suddenly appear";
		}
		message << "!";

		messageQueue.addMessage({ message.str() });
	}

	return true;
}