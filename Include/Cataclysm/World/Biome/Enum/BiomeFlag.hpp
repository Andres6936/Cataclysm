// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_BIOMEFLAG_HPP
#define CATACLYSM_BIOMEFLAG_HPP

#include <string>

enum Biome_flag
{
	BIOME_FLAG_NULL = 0,
	BIOME_FLAG_LAKE,    // "lake" - turn to ocean if ocean-adjacent
	BIOME_FLAG_CITY,    // "city" - turn into city buildings
	BIOME_FLAG_NO_OCEAN,// "no_ocean" - don't turn into ocean even if altitude<=0
	BIOME_FLAG_MAX
};

Biome_flag lookup_biome_flag(std::string name);

std::string biome_flag_name(Biome_flag flag);

#endif //CATACLYSM_BIOMEFLAG_HPP
