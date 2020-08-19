#include "Cataclysm/monster_ability.h"
#include "Cataclysm/geometry.h" // For Tripoint
#include "Cataclysm/monster.h"
#include "Cataclysm/game.h"
#include "Cataclysm/globals.h"
#include "Cataclysm/rng.h"
#include <sstream>
#include <map> // To help with messages

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

Monster_ability_summon::Monster_ability_summon()
{
	number = Dice(0, 0, 1);
	range = 1;
	max_summons = 0;
	verb = "creates";
}

Monster_ability_signal::Monster_ability_signal()
{
	range = 1;
}

Monster_ability_terrain::Monster_ability_terrain()
{
	always_replace = false;
	range = 1;
	tiles_affected = Dice(0, 0, 1);
	verb = "creates";
}

Monster_ability_teleport::Monster_ability_teleport()
{
	range = 1;
	always_use_max_range = false;
	controlled = false;
	phase = false;
	verb = "creates";
}

Monster_ability_fields::Monster_ability_fields()
{
	range = 1;
	affect_all_tiles = false;
	affect_self = false;
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
			if (GAME.is_empty(p))
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
		GAME.entities.add_entity(mon);

// Track spawn counts, for the monster below.
		if (GAME.player->can_sense(mon))
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
	bool see_user = GAME.player->can_sense(user);
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
		GAME.add_msg(message.str());
	}

	return true;
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
			if (GAME.player->can_see(GAME.map, ter_pos))
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
		GAME.add_msg(message.str());
	}

	return true;
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
				if (p != user->pos && GAME.is_empty(p) &&
					(phase || (user->can_see(GAME.map, p) &&
							   GAME.map->clear_path_exists(user->pos, p))))
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
				if (GAME.is_empty(p) &&
					(phase || (user->can_see(GAME.map, p) &&
							   GAME.map->clear_path_exists(user->pos, p))))
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

	bool see_orig = GAME.player->can_sense(user);

	int index = rng(0, valid_targets.size() - 1);
	Tripoint new_pos = valid_targets[index];
	user->pos = new_pos;

	bool see_new = GAME.player->can_sense(user);

	std::stringstream message;
	if (see_orig)
	{
		message << user->get_name_to_player() << " " << verb;
		if (see_new)
		{
			if (rl_dist(user->pos, GAME.player->pos) == 1)
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
		if (rl_dist(user->pos, GAME.player->pos) == 1)
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

bool Monster_ability_fields::handle_data(std::string ident, std::istream& data,
		std::string owner)
{
	std::string junk;

	if (ident == "range:")
	{
		data >> range;
		std::getline(data, junk);

	}
	else if (ident == "affect_all_tiles")
	{
		affect_all_tiles = true;

	}
	else if (ident == "affect_self")
	{
		affect_self = true;

	}
	else if (ident == "tiles_affected")
	{
		if (!tiles_affected.load_data(data, owner + " Monster_ability_fields"))
		{
			return false;
		}

	}
	else if (ident == "field_type:")
	{
		std::string line;
		std::getline(data, line);
		std::istringstream field_data(line);
		if (!field_type.load_data(field_data, owner + " Monster_ability_fields"))
		{
			return false;
		}

	}
	else if (ident == "duration:")
	{
		if (!duration.load_data(data, owner + " Monster_ability_fields"))
		{
			return false;
		}

	}
	else
	{
		debugmsg("Unknown Monster_ability_fields property '%s' (%s)",
				ident.c_str(), owner.c_str());
		return false;
	}

	return true;
}

bool Monster_ability_fields::effect(Monster* user)
{
	if (!user)
	{
		debugmsg("Monster_ability_fields:effect(NULL) called!");
		return false;
	}

	if (range <= 0)
	{
		debugmsg("%s called Monster_ability_fields::effect() with range %d!",
				user->get_data_name().c_str(), range);
		return false;
	}


	std::vector<Tripoint> valid_targets;
	Tripoint p;
	for (p.x = user->pos.x - range; p.x <= user->pos.x + range; p.x++)
	{
		for (p.y = user->pos.y - range; p.y <= user->pos.y + range; p.y++)
		{
			p.z = user->pos.z;
			if (GAME.is_empty(p) && (affect_self || p != user->pos))
			{
				valid_targets.push_back(p);
			}
		}
	}

	if (valid_targets.empty())
	{
		return false; // Cancel attempt
	}

	int num_fields = (affect_all_tiles ? 9999 : tiles_affected.roll());

	while (num_fields > 0 && !valid_targets.empty())
	{
		num_fields--;

		std::string field_name = field_type.pick();
		Field_type* type = FIELDS.lookup_name(field_name);
		if (!type)
		{
			debugmsg("%s tried to place non-existance field '%s'.",
					user->get_data_name().c_str(), field_name.c_str());
			return false;
		}

		int index = rng(0, valid_targets.size() - 1);
		Tripoint target = valid_targets[index];
		valid_targets.erase(valid_targets.begin() + index);

		int dur = duration.roll();

		if (dur > 0)
		{
			Field field(type);
			field.set_duration(dur);  // This will set its level appropriately
			GAME.map->add_field(field, target);
		}
	}

// Do we need messages for this skill?  Seems like it'd often be spammy.
// Skipping it for now.

	return true;
}

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
				if (ent == GAME.player)
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

	GAME.add_msg(mes.str());
	return true;
}

Monster_ability_type lookup_monster_ability_type(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 1; i < MON_ABILITY_MAX; i++)
	{
		Monster_ability_type ret = Monster_ability_type(i);
		if (no_caps(monster_ability_type_name(ret)) == name)
		{
			return ret;
		}
	}
	return MON_ABILITY_NULL;
}

std::string monster_ability_type_name(Monster_ability_type type)
{
	switch (type)
	{
	case MON_ABILITY_NULL:
		return "NULL";
	case MON_ABILITY_SUMMON:
		return "summon";
	case MON_ABILITY_SIGNAL:
		return "signal";
	case MON_ABILITY_TERRAIN:
		return "terrain";
	case MON_ABILITY_TELEPORT:
		return "teleport";
	case MON_ABILITY_FIELDS:
		return "fields";
	case MON_ABILITY_MAX:
		return "ERROR - MON_ABILITY_MAX";
	default:
		return "ERROR - Unnamed Monster_ability_type";
	}
	return "ERROR - Escaped monster_ability_type_name() switch";
}
