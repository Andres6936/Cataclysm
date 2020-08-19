// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TERRAINFLAG_HPP
#define CATACLYSM_TERRAINFLAG_HPP

#include <string>

enum World_terrain_flag
{
	WTF_NULL = 0,
	WTF_WATER,      // For river building
	WTF_NO_RIVER,   // For river building
	WTF_SALTY,      // Water is salty; also marks end of a river
	WTF_BRIDGE,     // Road on top of this tile is a bridge
	WTF_NO_ROAD,    // Roads don't draw here - but the path continues anyway
	WTF_LINE_DRAWING, // Use line drawings as glyphs
	WTF_RELATIONAL, // Drawing depends on adjacent terrain of same type
	WTF_SHOP,       // Is a shop - for world map generation
	WTF_FACE_ROAD,  // Rotate to face the road
	WTF_ROAD,       // Is a road, for the purpose of face_road
	WTF_NO_SURROUND,// For bonuses/spreading; never surround it fully
	WTF_MAX
};

World_terrain_flag lookup_world_terrain_flag(std::string name);

std::string world_terrain_flag_name(World_terrain_flag flag);

#endif //CATACLYSM_TERRAINFLAG_HPP
