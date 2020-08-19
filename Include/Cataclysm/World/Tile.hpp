// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TILE_HPP
#define CATACLYSM_TILE_HPP

#include <Cataclysm/glyph.h>
#include <Cataclysm/monster_spawn.h>
#include "Cataclysm/World/world_terrain.h"
#include <Cataclysm/Enum/TerrainFlag.hpp>

struct Worldmap_tile
{
	World_terrain* terrain;
	std::vector<Monster_spawn> monsters;

	glyph top_glyph();

	std::string get_name();

	bool has_flag(World_terrain_flag flag);

	void set_terrain(std::string name);

	std::string save_data();

	void load_data(std::istream& data);
};

#endif //CATACLYSM_TILE_HPP
