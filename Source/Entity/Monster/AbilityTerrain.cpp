// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Random/rng.h>
#include <Cataclysm/Entity/Monster/monster.h>
#include <Cataclysm/Visual/Screen/MessageQueue.hpp>
#include "Cataclysm/Entity/Monster/AbilityTerrain.hpp"

Monster_ability_terrain::Monster_ability_terrain()
{
	always_replace = false;
	range = 1;
	tiles_affected = Dice(0, 0, 1);
	verb = "creates";
}


bool Monster_ability_terrain::handle_data(std::string ident, std::istream& data,
		std::string owner)
{
	std::string junk;

	if (ident == "always_replace")
	{
		always_replace = true;

	}
	else if (ident == "damage:")
	{
		if (!damage.load_data(data, owner + " Monster_ability_terrain"))
		{
			return false;
		}

	}
	else if (ident == "terrain:")
	{
		std::string line;
		std::getline(data, line);
		std::istringstream terrain_data(line);
		if (!terrain.load_data(terrain_data, owner + " Monster_ability_terrain"))
		{
			return false;
		}

	}
	else if (ident == "range:")
	{
		data >> range;
		std::getline(data, junk);

	}
	else if (ident == "tiles_affected:")
	{
		if (!tiles_affected.load_data(data, owner + " Monster_ability_terrain"))
		{
			return false;
		}

	}
	else
	{
		debugmsg("Unknown Monster_ability_terrain property '%s' (%s).",
				ident.c_str(), owner.c_str());
		return false;
	}
	return true;
}

bool Monster_ability_terrain::effect(Monster* user)
{
	if (!user)
	{
		debugmsg("Monster_ability_terrain::effect(NULL) called!");
		return false;
	}

// Note: an empty terrain list is deliberately allowed here!

	if (range <= 0)
	{
		debugmsg("%s used Monster_ability_terrain but range was %d!",
				user->get_data_name().c_str(), range);
		return false;
	}

	int tiles = tiles_affected.roll();
// We return true here because it's an *attempt* to use the ability that failed.
	if (tiles < 0)
	{
		return true;
	}

	std::vector<Tripoint> valid_targets;
	Tripoint pos;
	for (pos.x = user->pos.x - range; pos.x <= user->pos.x + range; pos.x++)
	{
		for (pos.y = user->pos.y - range; pos.y <= user->pos.y + range; pos.y++)
		{
			pos.z = user->pos.z;
			if (pos == user->pos)
			{ // Skip terrain the caster is on!
				pos.y++;
			}
			if (!GAME.entities.entity_at(pos))
			{
				valid_targets.push_back(pos);
			}
		}
	}

	if (valid_targets.empty())
	{
		return false; // No valid targets, so cancel the attempt.
	}

	std::map<std::string, int> effects; // For printing a message.;
	while (tiles > 0 && !valid_targets.empty())
	{
		tiles--;

		int index = rng(0, valid_targets.size() - 1);
		Tripoint ter_pos = valid_targets[index];
		valid_targets.erase(valid_targets.begin() + index);

		std::string ter_name;
		Terrain* ter_used = NULL;
		if (!terrain.empty())
		{
			ter_name = terrain.pick();
			ter_used = TERRAIN.lookup_name(ter_name);
			if (!ter_used)
			{
				debugmsg("'%s' is not a terrain! (%s Monster_ability_terrain)",
						ter_name.c_str(), user->get_data_name().c_str());
				return false;
			}
		}

		Tile* tile_hit = GAME.map->get_tile(ter_pos);
		if (!tile_hit)
		{
			debugmsg("%s used Monster_ability_terrain and hit a NULL tile at %s!",
					user->get_data_name().c_str(), ter_pos.str().c_str());
			return false;
		}

		if (!always_replace)
		{
			tile_hit->damage(DAMAGE_BASH, damage.roll());
		}
		if (ter_used && (always_replace || tile_hit->hp <= 0))
		{
			tile_hit->set_terrain(ter_used);
// Check if we need to list the terrain
			if (GAME.player->can_see(GAME.map.get(), ter_pos))
			{
				if (effects.count(ter_name) == 0)
				{
					effects[ter_name] = 1;
				}
				else
				{
					effects[ter_name]++;
				}
			}
		}
	}

// Done!  Now display a message, if appropriate.
	bool see_user = GAME.player->can_sense(user);
	int nspn = effects.size();
	if (nspn > 0)
	{
		std::stringstream message;
		if (see_user)
		{
			message << user->get_name_to_player() << " " << verb << " ";
		}
		bool need_and = (nspn > 1);
		for (std::map<std::string, int>::iterator it = effects.begin();
			 it != effects.end();
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