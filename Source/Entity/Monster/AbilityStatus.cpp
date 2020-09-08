#include <Cataclysm/Util/String/stringfunc.h>
#include <Cataclysm/Util/globals.h>
#include "Cataclysm/Screen/Debugger.hpp"
#include <Cataclysm/Entity/Monster/monster.h>
#include <Cataclysm/Visual/Screen/MessageQueue.hpp>
#include "Cataclysm/Entity/Monster/AbilityStatus.h"

Monster_ability_status::Monster_ability_status()
{
	status = STATUS_NULL;
	range = 0;
	vector = BODY_PART_NULL;
	strength = 0;
	duration = Dice(0, 0, 1);
	level = Dice(0, 0, 1);
	affect_friends = false;
	affect_enemies = false;
	affect_self = false;
}

bool Monster_ability_status::handle_data(std::string ident, std::istream& data,
		std::string owner)
{
	std::string junk;

	if (ident == "status:")
	{
		std::string status_name;
		std::getline(data, status_name);
		status_name = trim(no_caps(status_name));
		status = lookup_status_effect(status_name);
		if (status == STATUS_NULL)
		{
			debugmsg("Unknown status '%s' in Monster_ability_status (%s).",
					status_name.c_str(), owner.c_str());
			return false;
		}

	}
	else if (ident == "range:")
	{
		data >> range;
		std::getline(data, junk);
		if (range < 0)
		{
			debugmsg("Bad range %d in Monster_ability_status (%s).",
					range, owner.c_str());
			return false;
		}

	}
	else if (ident == "vector:")
	{
		std::string body_part_name;
		std::getline(data, body_part_name);
		body_part_name = trim(no_caps(body_part_name));
		vector = lookup_body_part(body_part_name);
		if (vector == BODY_PART_NULL)
		{
			debugmsg("Unknown body part '%s' in Monster_ability_status (%s).",
					body_part_name.c_str(), owner.c_str());
			return false;
		}

	}
	else if (ident == "strength:")
	{
		data >> strength;
		std::getline(data, junk);
		if (strength < 0)
		{
			debugmsg("Bad strength %d in Monster_ability_status (%s).",
					strength, owner.c_str());
			return false;
		}

	}
	else if (ident == "duration:")
	{
		if (!duration.load_data(data, owner + " Monster_ability_status"))
		{
			return false;
		}

	}
	else if (ident == "level:")
	{
		if (!level.load_data(data, owner + " Monster_ability_status"))
		{
			return false;
		}

	}
	else if (ident == "affect_friends")
	{
		affect_friends = true;

	}
	else if (ident == "affect_enemies")
	{
		affect_enemies = true;

	}
	else if (ident == "affect_self")
	{
		affect_self = true;

	}
	else
	{
		debugmsg("Unknown Monster_ability_status property '%s' (%s).",
				ident.c_str(), owner.c_str());
		return false;
	}

	return true;
}

bool Monster_ability_status::effect(Monster* user)
{
	if (!user)
	{
		debugmsg("Monster_ability_status::effect(NULL) called!");
		return false;
	}

// Detect all valid targets.
	std::vector<Entity*> targets;
	Tripoint p;
	std::map<std::string, int> target_types; // For messages below
	for (p.x = user->pos.x - range; p.x <= user->pos.x + range; p.x++)
	{
		for (p.y = user->pos.y - range; p.y <= user->pos.y + range; p.y++)
		{
			p.z = user->pos.z;
			Entity* ent = GAME.entities.entity_at(p);
			if (ent)
			{
				std::string ent_name = ent->get_name();
				if (ent == player.get())
				{
					ent_name = "you";
				}
				bool use_ent = false;
				if (ent == user)
				{  // It's us!
					if (affect_self)
					{
						use_ent = true;
					}
				}
				else if (user->is_enemy(ent))
				{
					if (affect_enemies)
					{
						use_ent = true;
					}
				}
				else
				{  // Not an enemy and not ourselfs; must be a friend!
					if (affect_friends)
					{
						use_ent = true;
					}
				}

				if (use_ent)
				{
					targets.push_back(ent);
					if (target_types.count(ent_name) == 0)
					{
						target_types[ent_name] = 1;
					}
					else
					{
						target_types[ent_name]++;
					}
				}
			}
		}
	}

	if (targets.empty())
	{  // Couldn't find anything!
		return false;
	}

	bool force = (vector == BODY_PART_NULL || strength == 0); // No chance to save
	for (int i = 0; i < targets.size(); i++)
	{
		int dur = duration.roll(), lev = level.roll();
		if (force)
		{
			targets[i]->add_status_effect(status, dur, lev);
		}
		else
		{
			targets[i]->infect(vector, strength, status, dur, lev);
		}
	}

// Finally, add a message.
	std::stringstream mes;
	int num = target_types.size();
	bool need_and = (num > 1);
	mes << user->get_name_to_player() << " " << verb << " ";
	for (std::map<std::string, int>::iterator it = target_types.begin();
		 it != target_types.end();
		 it++)
	{
		num--;
		if (num <= 0 && need_and)
		{
			mes << "and ";
		}
		if (it->first == "you")
		{
			mes << "<c=yellow>you<c=/>";
		}
		else if (it->second == 1)
		{
			mes << "a " << it->first;
		}
		else
		{
			mes << it->second << " " << it->first << "s";
		}
		if (num == 1)
		{
			mes << " ";
		}
		else if (num > 1)
		{
			mes << ", ";
		}
	}

	messageQueue.addMessage({ mes.str() });
	return true;
}
