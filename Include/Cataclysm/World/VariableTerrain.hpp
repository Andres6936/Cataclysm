// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_VARIABLETERRAIN_HPP
#define CATACLYSM_VARIABLETERRAIN_HPP

#include <string>
#include <sstream>
#include <vector>

#include "Cataclysm/World/TerrainChance.hpp"
#include "Cataclysm/World/world_terrain.h"

struct Variable_world_terrain
{
public:
	Variable_world_terrain();

	~Variable_world_terrain()
	{
	}

	void add_terrain(int chance, World_terrain* terrain);

	void add_terrain(World_terrain_chance terrain);

	bool load_data(std::istream& data, std::string name = "unknown");

	bool empty();

	World_terrain* pick();

private:
	std::vector<World_terrain_chance> ter;
	int total_chance;
};

#endif //CATACLYSM_VARIABLETERRAIN_HPP
