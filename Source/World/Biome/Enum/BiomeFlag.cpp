// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Biome/Enum/BiomeFlag.hpp"
#include <Cataclysm/Util/String.hpp>

Biome_flag lookup_biome_flag(std::string name)
{
	name = no_caps(name);
	for (int i = 0; i < BIOME_FLAG_MAX; i++)
	{
		Biome_flag ret = Biome_flag(i);
		if (no_caps(biome_flag_name(ret)) == name)
		{
			return ret;
		}
	}
	return BIOME_FLAG_NULL;
}

std::string biome_flag_name(Biome_flag flag)
{
	switch (flag)
	{
	case BIOME_FLAG_NULL:
		return "NULL";
	case BIOME_FLAG_LAKE:
		return "lake";
	case BIOME_FLAG_CITY:
		return "city";
	case BIOME_FLAG_NO_OCEAN:
		return "no_ocean";
	case BIOME_FLAG_MAX:
		return "ERROR - BIOME_FLAG_MAX";
	default:
		return "ERROR - Unnamed Biome_flag";
	}
	return "ERROR - Escaped switch";
}


