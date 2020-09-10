// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Biome/VariableMonsterGenus.hpp"
#include <Cataclysm/Screen/Debugger.hpp>
#include <Cataclysm/Util/String/String.hpp>
#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Random/rng.h>


Variable_monster_genus::Variable_monster_genus()
{
	total_chance = 0;
}

Variable_monster_genus::~Variable_monster_genus()
{
}

void Variable_monster_genus::add_genus(int chance, Monster_genus* genus)
{
	if (!genus)
	{
		return;
	}
	add_genus(Monster_genus_chance(chance, genus));
}

void Variable_monster_genus::add_genus(Monster_genus_chance genus)
{
	total_chance += genus.chance;
	genera.push_back(genus);
}

bool Variable_monster_genus::load_data(std::istream& data, std::string name)
{
	std::string ident;
	std::string genus_name;
	Monster_genus_chance tmp_chance;
	while (data >> ident)
	{
		ident = no_caps(ident);
		if (ident.substr(0, 2) == "w:")
		{ // It's a weight, i.e. a chance
			tmp_chance.chance = atoi(ident.substr(2).c_str());
		}
		else if (ident == "/")
		{ // End of this option
			genus_name = trim(genus_name);
			Monster_genus* tmpgenus = MONSTER_GENERA.lookup_name(genus_name);
			if (!tmpgenus)
			{
				debugmsg("Unknown genus '%s' (%s)", genus_name.c_str(), name.c_str());
				return false;
			}
			tmp_chance.genus = tmpgenus;
			add_genus(tmp_chance);
			tmp_chance.chance = 10;
			tmp_chance.genus = NULL;
			genus_name = "";
		}
		else
		{ // Otherwise it should be a terrain name
			genus_name = genus_name + " " + ident;
		}
	}
// Add the last genus to our list
	genus_name = trim(genus_name);
	Monster_genus* tmpgenus = MONSTER_GENERA.lookup_name(genus_name);
	tmp_chance.genus = tmpgenus;
	if (!tmpgenus)
	{
		debugmsg("Unknown world terrain '%s' (%s)", genus_name.c_str(),
				name.c_str());
		return false;
	}
	add_genus(tmp_chance);
	return true;
}

int Variable_monster_genus::size()
{
	return genera.size();
}

Monster_genus* Variable_monster_genus::pick()
{
	if (genera.empty())
	{
		return NULL;
	}

	int index = rng(1, total_chance);
	for (int i = 0; i < genera.size(); i++)
	{
		index -= genera[i].chance;
		if (index <= 0)
		{
			return genera[i].genus;
		}
	}
	return genera.back().genus;
}

std::vector<Monster_genus*> Variable_monster_genus::pick(int num)
{
	std::vector<Monster_genus*> ret;

	if (genera.empty())
	{
		return ret;
	}

	if (num >= genera.size())
	{
		for (int i = 0; i < genera.size(); i++)
		{
			ret.push_back(genera[i].genus);
		}
		return ret;
	}

	std::vector<bool> picked;
	for (int i = 0; i < genera.size(); i++)
	{
		picked.push_back(false);
	}

	int new_total = total_chance;
	while (ret.size() < num)
	{
		int index = rng(1, new_total);
		for (int i = 0; i < genera.size(); i++)
		{
// Skip any we've already picked
			while (i < genera.size() && picked[i])
			{
				i++;
			}
			index -= genera[i].chance;
			if (index <= 0)
			{
				picked[i] = true;
				new_total -= genera[i].chance;
				ret.push_back(genera[i].genus);
			}
		}
	}
	return ret;
}

int Variable_monster_genus::pick_number()
{
	int ret = 0;
	while (rng(1, size()) > ret)
	{
		ret++;
	}
	return ret;
}

