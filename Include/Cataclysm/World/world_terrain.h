#ifndef _WORLD_TERRAIN_H_
#define _WORLD_TERRAIN_H_

#include <string>
#include <vector>

#include <Cataclysm/Graphics/glyph.h>
#include <Cataclysm/Geometry/geometry.h>
#include <Cataclysm/World/Enum/SpreadType.hpp>
#include <Cataclysm/World/Enum/TerrainFlag.hpp>

struct World_terrain
{
	int uid;
	std::string name; // Unique data name
	std::string display_name; // Name as the player sees it (if blank, use name)
	std::string beach_name; // What we become when we're near to the ocean
// Any names in (connectors) count as "the same" as this tile, for Relationals
	std::vector<std::string> connectors;
	int beach_range;  // The maximum distance to ocean to become beach
	int road_cost;  // Used when pathing roads; high cost means roads go around
	int spread_cost;  // Resistance to being covered by bonuses
	Dice spread; // How much spread_cost we can cover when placed as a bonus
	Spread_type spread_type;  // How do we spread?  Defaults to SPREAD_NORMAL
/* We use a string in order to allow us to include ourselves and not-yet-defined
 * terrain.  When the spread_options is used (in worldmap_generate.cpp,
 * Worldmap::add_bonus()), we'll convert it to a Variable_world_terrain.
 */
	std::string spread_options;
	glyph sym;

	World_terrain();

	~World_terrain()
	{
	}

	std::string get_data_name();

	std::string get_name();

	void assign_uid(int id)
	{
		uid = id;
	}

	bool load_data(std::istream& data);

	bool has_flag(World_terrain_flag flag);

private:
	std::vector<bool> flags;
};

World_terrain* make_into_beach(World_terrain* original);

#endif
