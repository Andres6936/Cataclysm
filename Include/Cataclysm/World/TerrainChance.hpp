// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TERRAINCHANCE_HPP
#define CATACLYSM_TERRAINCHANCE_HPP

class World_terrain;

struct World_terrain_chance
{
	World_terrain_chance(int C = 100, World_terrain* WT = NULL) :
			chance(C), terrain(WT)
	{
	}

	int chance;
	World_terrain* terrain;
};

#endif //CATACLYSM_TERRAINCHANCE_HPP
