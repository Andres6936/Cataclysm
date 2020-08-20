// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Tile.hpp"
#include "Cataclysm/globals.h"

glyph Worldmap_tile::top_glyph()
{
	if (!terrain)
	{
		return glyph();
	}
	return terrain->sym;
}

std::string Worldmap_tile::get_name()
{
	if (!terrain)
	{
		return "Unknown";
	}
	return terrain->get_name();
}

bool Worldmap_tile::has_flag(World_terrain_flag flag)
{
	if (!terrain)
	{
		return false;
	}
	return terrain->has_flag(flag);
}

void Worldmap_tile::set_terrain(std::string name)
{
	World_terrain* ter = WORLD_TERRAIN.lookup_name(name);
	if (!ter)
	{
		debugmsg("Couldn't find world terrain named '%s'", name.c_str());
	}
	else
	{
		terrain = ter;
	}
}

std::string Worldmap_tile::save_data()
{
	std::stringstream ret;
	if (terrain)
	{
		ret << terrain->uid;
	}
	else
	{
		ret << -1;
	}
	ret << " " << monsters.size() << " ";
	for (int i = 0; i < monsters.size(); i++)
	{
		if (monsters[i].genus)
		{
			ret << monsters[i].genus->uid;
		}
		else
		{
			ret << -1;
		}
		ret << " " << monsters[i].population << " ";
	}
	return ret.str();
}

void Worldmap_tile::load_data(std::istream& data)
{
	int tmpter;
	data >> tmpter;
	if (tmpter == -1)
	{
		terrain = NULL;
	}
	else
	{
		terrain = WORLD_TERRAIN.lookup_uid(tmpter);
	}
	int monster_size;
	data >> monster_size;
	for (int i = 0; i < monster_size; i++)
	{
		Monster_spawn tmpspawn;
		int tmpgenus;
		data >> tmpgenus;
		if (tmpgenus == -1)
		{
			tmpspawn.genus = NULL;
		}
		else
		{
			tmpspawn.genus = MONSTER_GENERA.lookup_uid(tmpgenus);
		}
		data >> tmpspawn.population;
		if (tmpspawn.genus)
		{
			monsters.push_back(tmpspawn);
		}
	}
}


