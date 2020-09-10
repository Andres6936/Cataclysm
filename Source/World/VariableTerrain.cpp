// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/VariableTerrain.hpp"
#include <Cataclysm/Random/rng.h>
#include <Cataclysm/Util/String/String.hpp>
#include <Cataclysm/Util/globals.h>

Variable_world_terrain::Variable_world_terrain()
{
	total_chance = 0;
}

void Variable_world_terrain::add_terrain(int chance, World_terrain* terrain)
{
	if (!terrain)
	{
		return;
	}
	World_terrain_chance tmp(chance, terrain);
	add_terrain(tmp);
}

void Variable_world_terrain::add_terrain(World_terrain_chance terrain)
{
	total_chance += terrain.chance;
	ter.push_back(terrain);
}

bool Variable_world_terrain::load_data(std::istream& data, std::string name)
{
	std::string tile_ident;
	std::string terrain_name;
	World_terrain_chance tmp_chance;
	while (data >> tile_ident)
	{
		tile_ident = no_caps(tile_ident);
		if (tile_ident.substr(0, 2) == "w:")
		{ // It's a weight, i.e. a chance
			tmp_chance.chance = atoi(tile_ident.substr(2).c_str());
		}
		else if (tile_ident == "/")
		{ // End of this option
			terrain_name = trim(terrain_name);
			World_terrain* tmpter = WORLD_TERRAIN.lookup_name(terrain_name);
			if (!tmpter)
			{
				debugmsg("Unknown world terrain '%s' (%s)", terrain_name.c_str(),
						name.c_str());
				return false;
			}
			tmp_chance.terrain = tmpter;
			add_terrain(tmp_chance);
			tmp_chance.chance = 10;
			tmp_chance.terrain = NULL;
			terrain_name = "";
		}
		else
		{ // Otherwise it should be a terrain name
			terrain_name = terrain_name + " " + tile_ident;
		}
	}
// Add the last terrain def to our list
	terrain_name = trim(terrain_name);
	World_terrain* tmpter = WORLD_TERRAIN.lookup_name(terrain_name);
	tmp_chance.terrain = tmpter;
	if (!tmpter)
	{
		debugmsg("Unknown world terrain '%s' (%s)", terrain_name.c_str(),
				name.c_str());
		return false;
	}
	add_terrain(tmp_chance);
	return true;
}

bool Variable_world_terrain::empty()
{
	return ter.empty();
}

World_terrain* Variable_world_terrain::pick()
{
	if (ter.empty())
	{
		return NULL;
	}

	int index = rng(1, total_chance);
	for (int i = 0; i < ter.size(); i++)
	{
		if (i < 0 || i >= ter.size())
		{
			debugmsg("i = %d, ter.size() = %d", i, ter.size());
		}
		index -= ter[i].chance;
		if (index <= 0)
		{
			return ter[i].terrain;
		}
	}
	return ter.back().terrain;
}


