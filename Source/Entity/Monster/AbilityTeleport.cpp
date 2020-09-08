// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Random/rng.h>
#include "Cataclysm/Screen/Debugger.hpp"
#include <Cataclysm/World/ObserverMap.hpp>
#include <Cataclysm/Entity/Monster/monster.h>
#include "Cataclysm/Entity/Monster/AbilityTeleport.hpp"

Monster_ability_teleport::Monster_ability_teleport()
{
	range = 1;
	always_use_max_range = false;
	controlled = false;
	phase = false;
	verb = "creates";
}



bool Monster_ability_teleport::handle_data(std::string ident,
		std::istream& data,
		std::string owner)
{
	std::string junk;

	if (ident == "range:")
	{
		data >> range;
		std::getline(data, junk);

	}
	else if (ident == "always_use_max_range")
	{
		always_use_max_range = true;

	}
	else if (ident == "controlled")
	{
		controlled = true;

	}
	else if (ident == "phase")
	{
		phase = true;

	}
	else
	{
		debugmsg("Unknown Monster_ability_teleport property '%s' (%s)",
				ident.c_str(), owner.c_str());
		return false;
	}
	return true;
}

bool Monster_ability_teleport::effect(Monster* user)
{
	if (!user)
	{
		debugmsg("Monster_ability_teleport::effect(NULL) called!");
		return false;
	}

	if (range <= 0)
	{
		debugmsg("%s called Monster_ability_teleport with range %d!",
				user->get_data_name().c_str(), range);
		return false;
	}

	std::vector<Tripoint> valid_targets;

	Tripoint p;

	bool fleeing = user->is_fleeing();
	Tripoint target = user->plan.get_target();
	if (always_use_max_range)
	{
		p.z = user->pos.z;
		for (int diff = 0 - range; diff <= range; diff++)
		{
			for (int n = 0; n < 4; n++)
			{
// We're tracing the edge of a square - n decides which edge.
				switch (n)
				{
				case 0:
					p.x = user->pos.x + diff;
					p.y = user->pos.y - range;
					break;
				case 1:
					p.x = user->pos.x + diff;
					p.y = user->pos.y + range;
					break;
				case 2:
					p.x = user->pos.x - range;
					p.y = user->pos.y + diff;
					break;
				case 3:
					p.x = user->pos.x + range;
					p.y = user->pos.y + diff;
					break;
				}
				if (p != user->pos && observerMap.isEmpty(p) &&
					(phase || (user->can_see(map.get(), p) &&
							   map->clear_path_exists(user->pos, p))))
				{
// If controlled, ensure we're getting closer to / further from our target
					if (controlled && target.x > 0)
					{
						int orig_dist = rl_dist(user->pos, target),
								new_dist = rl_dist(p, target);
						if ((fleeing && orig_dist >= new_dist) ||
							(!fleeing && orig_dist <= new_dist))
						{
							valid_targets.push_back(p);
						}
					}
					else
					{ // Not controlled / no target, so who cares
						valid_targets.push_back(p);
					}
				}
			}
		}
	}
	else
	{ // if (always_use_max_range)
		for (p.x = user->pos.x - range; p.x <= user->pos.x + range; p.x++)
		{
			for (p.y = user->pos.y - range; p.y <= user->pos.y + range; p.y++)
			{
				p.z = user->pos.z;
				if (p == user->pos)
				{ // Skip central tile
					p.y++;
				}
				if (observerMap.isEmpty(p) &&
					(phase || (user->can_see(map.get(), p) &&
							   map->clear_path_exists(user->pos, p))))
				{
// If controlled, ensure we're getting closer to / further from our target
					if (controlled && target.x > 0)
					{
						int orig_dist = rl_dist(user->pos, target),
								new_dist = rl_dist(p, target);
						if ((fleeing && orig_dist >= new_dist) ||
							(!fleeing && orig_dist <= new_dist))
						{
							valid_targets.push_back(p);
						}
					}
					else
					{ // Not controlled / no target, so who cares
						valid_targets.push_back(p);
					}
				}
			}
		}
	}

	if (valid_targets.empty())
	{
		return false; // No valid teleport points, so cancel the attempt.
	}

	bool see_orig = player->can_sense(user);

	int index = rng(0, valid_targets.size() - 1);
	Tripoint new_pos = valid_targets[index];
	user->pos = new_pos;

	bool see_new = player->can_sense(user);

	std::stringstream message;
	if (see_orig)
	{
		message << user->get_name_to_player() << " " << verb;
		if (see_new)
		{
			if (rl_dist(user->pos, player->pos) == 1)
			{
				message << " right next to you!";
			}
			else
			{
				message << "!";
			}
		}
		else
		{
			message << " out of sight!";
		}
	}
	else if (see_new)
	{
		message << user->get_name_indefinite() << " " << verb << " ";
		if (rl_dist(user->pos, player->pos) == 1)
		{
			message << "right next to you!";
		}
		else
		{
			message << "into view!";
		}
	}
// Obviously, if we can't see their old OR their new position, no message.

	return true;
}