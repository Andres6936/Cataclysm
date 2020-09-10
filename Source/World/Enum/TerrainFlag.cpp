// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Enum/TerrainFlag.hpp"
#include <Cataclysm/Util/String.hpp>

World_terrain_flag lookup_world_terrain_flag(std::string name)
{
	name = no_caps(name);
	for (int i = 0; i < WTF_MAX; i++)
	{
		World_terrain_flag ret = World_terrain_flag(i);
		if (no_caps(world_terrain_flag_name(ret)) == name)
		{
			return ret;
		}
	}
	return WTF_NULL;
}

std::string world_terrain_flag_name(World_terrain_flag flag)
{
	switch (flag)
	{
	case WTF_NULL:
		return "NULL";
	case WTF_WATER:
		return "water";
	case WTF_NO_RIVER:
		return "no_river";
	case WTF_SALTY:
		return "salty";
	case WTF_BRIDGE:
		return "bridge";
	case WTF_NO_ROAD:
		return "no_road";
	case WTF_LINE_DRAWING:
		return "line_drawing";
	case WTF_RELATIONAL:
		return "relational";
	case WTF_SHOP:
		return "shop";
	case WTF_FACE_ROAD:
		return "face_road";
	case WTF_ROAD:
		return "road";
	case WTF_NO_SURROUND:
		return "no_surround";
	case WTF_MAX:
		return "BUG - WTF_MAX";
	default:
		return "BUG - Unnamed World_terrain_flag";
	}
	return "BUG - Escaped switch in world_terrain_flag_name()";
}

