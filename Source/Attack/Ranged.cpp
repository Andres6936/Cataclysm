// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Attack/Ranged.hpp"
#include "Cataclysm/Attack/Verbs.hpp"
#include "Cataclysm/stringfunc.h" // For no_caps
#include <Cataclysm/Screen/Debugger.hpp>
#include "Cataclysm/rng.h" // For rng

Ranged_attack::Ranged_attack()
{
	verb_second = "shoot";
	verb_third = "shoots";
	weight = 10;
	speed = 100;
	charge_time = 0;
	range = 0;
	rounds = 1;
	pellets = 1;
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		damage[i] = 0;
		armor_divisor[i] = 10;
	}
}

Ranged_attack::~Ranged_attack()
{
}

bool Ranged_attack::load_data(std::istream& data, std::string owner_name)
{
	std::string ident, junk;
	while (ident != "done" && !data.eof())
	{

		if (!(data >> ident))
		{
			return false;
		}

		ident = no_caps(ident);

		if (!ident.empty() && ident[0] == '#')
		{ // I'ts a comment
			std::getline(data, junk);

		}
		else if (ident == "verb_second:")
		{
			std::getline(data, verb_second);
			verb_second = trim(verb_second);

		}
		else if (ident == "verb_third:")
		{
			std::getline(data, verb_third);
			verb_third = trim(verb_third);

		}
		else if (ident == "verb:")
		{
			if (!load_verbs(data, verb_second, verb_third, owner_name))
			{
				return false;
			}

		}
		else if (ident == "weight:")
		{
			data >> weight;
			std::getline(data, junk);

		}
		else if (ident == "speed:")
		{
			data >> speed;
			std::getline(data, junk);

		}
		else if (ident == "charge:" || ident == "charge_time:")
		{
			data >> charge_time;
			std::getline(data, junk);

		}
		else if (ident == "range:")
		{
			data >> range;
			std::getline(data, junk);

		}
		else if (ident == "rounds:")
		{
			data >> rounds;
			std::getline(data, junk);

		}
		else if (ident == "pellets:")
		{
			data >> pellets;
			std::getline(data, junk);

		}
		else if (ident == "variance:")
		{
			if (!variance.load_data(data, owner_name))
			{
				return false;
			}

		}
		else if (ident == "armor_pierce:")
		{
			std::string damage_name;
			data >> damage_name;
			Damage_type damtype = lookup_damage_type(damage_name);
			if (damtype == DAMAGE_NULL)
			{
				debugmsg("Unknown damage type for Ranged_attack pierce: '%s' (%s)",
						damage_name.c_str(), owner_name.c_str());
				return false;
			}
			data >> armor_divisor[damtype];
			if (armor_divisor[damtype] == 0)
			{
				armor_divisor[damtype] = 10;
			}

		}
		else if (ident == "wake_field:")
		{
			if (!wake_field.load_data(data, owner_name + " " + verb_third))
			{
				debugmsg("Failed to load wake_field (%s)", owner_name.c_str());
				return false;
			}

		}
		else if (ident == "target_field:")
		{
			if (!target_field.load_data(data, owner_name + " " + verb_third))
			{
				debugmsg("Failed to load target_field (%s)", owner_name.c_str());
				return false;
			}

		}
		else if (ident != "done")
		{
			std::string damage_name = ident;
			size_t colon = ident.find(':');
			if (colon != std::string::npos)
			{
				damage_name = ident.substr(0, colon);
			}
			Damage_type type = lookup_damage_type(damage_name);
			if (type == DAMAGE_NULL)
			{
				debugmsg("Unknown Attack property '%s' (%s)",
						ident.c_str(), owner_name.c_str());
				return false;
			}
			else
			{
				data >> damage[type];
			}
		}

	}
	return true;
}

int Ranged_attack::roll_variance()
{
	return variance.roll();
}

Damage_set Ranged_attack::roll_damage(Ranged_hit_type hit)
{
	double min, max;
	switch (hit)
	{
	case RANGED_HIT_NULL: // Shouldn't ever happen
		min = 1.0;
		max = 1.0;
		break;
	case RANGED_HIT_GRAZE:
		min = 0.0;
		max = 1.0;
		break;
	case RANGED_HIT_NORMAL: // Default
		min = 0.8;
		max = 1.0;
		break;
	case RANGED_HIT_CRITICAL:
		min = 1.0;
		max = 2.5;
		break;
	case RANGED_HIT_HEADSHOT:
		min = 3.0;
		max = 5.0;
		break;
	}

	Damage_set ret;
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		ret.set_damage(Damage_type(i), rng(damage[i] * min, damage[i] * max));
	}
	return ret;
}
