// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Random/rng.h>
#include <Cataclysm/Entity/Monster/monster.h>
#include "Cataclysm/Entity/Monster/AbilityFields.hpp"

Monster_ability_fields::Monster_ability_fields()
{
	range = 1;
	affect_all_tiles = false;
	affect_self = false;
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
			map->add_field(field, target);
		}
	}

// Do we need messages for this skill?  Seems like it'd often be spammy.
// Skipping it for now.

	return true;
}