#include <Cataclysm/World/worldmap.h>
#include <Cataclysm/World/Biome/Enum/LakeStatus.hpp>
#include <Cataclysm/World/Biome/Enum/CityStatus.hpp>
#include "Cataclysm/Random/rng.h"
#include <Cataclysm/Util/globals.h>

void Worldmap::generate()
{
// Points_live is used below to track which points to update
	std::vector<Point> points_live;
// Used below when deciding when to turn lakes into ocean
	Lake_status lake[WORLDMAP_SIZE][WORLDMAP_SIZE];
	City_status city[WORLDMAP_SIZE][WORLDMAP_SIZE];
	std::vector<Point> lake_seeds;
	std::vector<Point> city_seeds;
	for (int x = 0; x < WORLDMAP_SIZE; x++)
	{
		for (int y = 0; y < WORLDMAP_SIZE; y++)
		{
			biomes[x][y] = NULL;
			lake[x][y] = LAKE_NOTLAKE;
			city[x][y] = CITY_NOTCITY;
		}
	}

	// Randomly seed biomes
	for (auto& biome : BIOMES)
	{
		for (int n = 0; n < WORLDMAP_SIZE / 10; n++)
		{
			Point p(rng(0, WORLDMAP_SIZE - 1), rng(0, WORLDMAP_SIZE - 1));
			points_live.push_back(p);
			if (biome->has_flag(BIOME_FLAG_LAKE))
			{
				lake_seeds.push_back(p);
				lake[p.x][p.y] = LAKE_UNCHECKED;
			}
			if (biome->has_flag(BIOME_FLAG_CITY))
			{
				city_seeds.push_back(p);
				city[p.x][p.y] = CITY_HUB;
			}
			biomes[p.x][p.y] = biome;
		}
	}

	while (!points_live.empty())
	{
		std::vector<Point> new_points;
		//std::vector<Point> points_copy = points_live;
		int i = rng(0, points_live.size() - 1);
		std::vector<Point> valid_growth;
		Point p = points_live[i];
		if (p.x > 0 && biomes[p.x - 1][p.y] == NULL)
		{
			valid_growth.push_back(Point(p.x - 1, p.y));
		}
		if (p.y > 0 && biomes[p.x][p.y - 1] == NULL)
		{
			valid_growth.push_back(Point(p.x, p.y - 1));
		}
		if (p.x < WORLDMAP_SIZE - 1 && biomes[p.x + 1][p.y] == NULL)
		{
			valid_growth.push_back(Point(p.x + 1, p.y));
		}
		if (p.y < WORLDMAP_SIZE - 1 && biomes[p.x][p.y + 1] == NULL)
		{
			valid_growth.push_back(Point(p.x, p.y + 1));
		}
		if (valid_growth.empty())
		{ // No valid points - this point is dead!
			points_live.erase(points_live.begin() + i);
			i--;
		}
		else
		{
			Point growth = valid_growth[rng(0, valid_growth.size() - 1)];
			biomes[growth.x][growth.y] = biomes[p.x][p.y];
			lake[growth.x][growth.y] = lake[p.x][p.y];
			if (city[p.x][p.y] == CITY_HUB)
			{
				city[growth.x][growth.y] = CITY_RAW;
			}
			else
			{
				city[growth.x][growth.y] = city[p.x][p.y];
			}
			points_live.push_back(growth);
		}
	}

// Now look at the biomes and randomly select a terrain for each
	for (int x = 0; x < WORLDMAP_SIZE; x++)
	{
		for (int y = 0; y < WORLDMAP_SIZE; y++)
		{
			if (biomes[x][y])
			{
				tiles[x][y].terrain = biomes[x][y]->pick_terrain();
			}
			else
			{
				tiles[x][y].set_terrain("ocean");
			}
		}
	}

/* At this point, we have a lot of blobs of terrain, but no ocean!
 * The draw_island function sets altitude to 100 at its center and randomly
 * slopes down in a way that introduces penisulas &c
 */

	int center = WORLDMAP_SIZE / 2, shift = WORLDMAP_SIZE / 10;
	Point island_center(rng(center - shift, center + shift),
			rng(center - shift, center + shift));
	std::vector<std::vector<int> > altitude;
	std::vector<int> tmpvec;
	for (int x = 0; x < WORLDMAP_SIZE; x++)
	{
		tmpvec.push_back(0);
	}
	for (int x = 0; x < WORLDMAP_SIZE; x++)
	{
		altitude.push_back(tmpvec);
	}

	draw_island(altitude, island_center, 400, 20, 0);

// Now draw several (8) more, small islands
	for (int i = 0; i < 8; i++)
	{
		Point islet;
		switch (rng(1, 4))
		{ // Which side to place it along?
		case 1:
			islet.x = rng(0, WORLDMAP_SIZE - 1);
			islet.y = rng(15, 40);
			break;
		case 2:
			islet.x = rng(WORLDMAP_SIZE - 41, WORLDMAP_SIZE - 16);
			islet.y = rng(0, WORLDMAP_SIZE - 1);
			break;
		case 3:
			islet.x = rng(0, WORLDMAP_SIZE - 1);
			islet.y = rng(WORLDMAP_SIZE - 41, WORLDMAP_SIZE - 16);
			break;
		case 4:
			islet.x = rng(15, 40);
			islet.y = rng(0, WORLDMAP_SIZE - 1);
			break;
		}
		int size = 80;
		draw_island(altitude, islet, size, 2, i + 1);
		while (one_in(3))
		{ // island chain
			if (one_in(2))
			{
				islet.x -= rng(size / 5, size / 3);
			}
			else
			{
				islet.x += rng(size / 5, size / 3);
			}
			if (one_in(2))
			{
				islet.y -= rng(size / 5, size / 3);
			}
			else
			{
				islet.y += rng(size / 5, size / 3);
			}
			size -= rng(0, 20);
/* Using "i + 1" as the ID means that all islands in the same chain are
 * considered to be the "same" island - for whatever purposes the island ID is
 * ultimately used for (e.g. random_tile_with_terrain()).  TODO: change???
 */
			draw_island(altitude, islet, size, 2, i + 1);
		}
	}

// Now find all lake biomes that are ocean-adjacent and make them shallows.
// Also, all the surviving lakes should become a river seed
	std::vector<Point> river_seeds;
	for (int i = 0; i < lake_seeds.size(); i++)
	{
		std::vector<Point> lake_points;
		std::vector<Point> live_points;
		lake_points.push_back(lake_seeds[i]);
		live_points.push_back(lake_seeds[i]);
		bool ocean = false;
		while (!live_points.empty())
		{
			Point p = live_points[0];
			for (int x = p.x - 1; x <= p.x + 1; x++)
			{
				for (int y = p.y - 1; y <= p.y + 1; y++)
				{
					if (x >= 0 && x < WORLDMAP_SIZE && y >= 0 && y < WORLDMAP_SIZE)
					{
						if (lake[x][y] == LAKE_UNCHECKED)
						{
							lake_points.push_back(Point(x, y));
							live_points.push_back(Point(x, y));
							lake[x][y] = LAKE_CHECKED;
						}
						else if (!ocean && altitude[x][y] <= 0)
						{
							ocean = true;
						}
					}
				}
			}
			live_points.erase(live_points.begin());
		}
		if (ocean)
		{
			for (int i = 0; i < lake_points.size(); i++)
			{
				Point p = lake_points[i];
				altitude[p.x][p.y] = 0;
				//set_terrain(p.x, p.y, "tester");
			}
		}
		else
		{
			river_seeds.push_back(lake_seeds[i]);
		}
	}

// For each river seed, draw a river that *tends* to slope down until it hits
// ocean.

	for (int i = 0; i < river_seeds.size(); i++)
	{
		Point rp = river_seeds[i];
		bool done = false;
// TODO: This occasionally fails to terminate.
		while (!done)
		{
			if (!tiles[rp.x][rp.y].terrain->has_flag(WTF_NO_RIVER) &&
				!tiles[rp.x][rp.y].terrain->has_flag(WTF_WATER))
			{
				tiles[rp.x][rp.y].set_terrain("river");
			}
			std::vector<Point> next;
			std::vector<int> chances;
			int total_chance = 0;
			for (int n = 1; n <= 4; n++)
			{
				int x, y;
				switch (n)
				{
				case 1:
					x = rp.x - 1;
					y = rp.y;
					break;
				case 2:
					x = rp.x + 1;
					y = rp.y;
					break;
				case 3:
					x = rp.x;
					y = rp.y - 1;
					break;
				case 4:
					x = rp.x;
					y = rp.y + 1;
					break;
				}
				if (x < 0 || x >= WORLDMAP_SIZE || y < 0 || y >= WORLDMAP_SIZE ||
					tiles[x][y].terrain->has_flag(WTF_SALTY) || altitude[x][y] <= 0)
				{
					done = true;
// no_river tiles are only acceptable if it has the water flag too
				}
				else if (!tiles[x][y].terrain->has_flag(WTF_NO_RIVER) ||
						 tiles[x][y].terrain->has_flag(WTF_WATER))
				{
					next.push_back(Point(x, y));
					int chance;
					if (altitude[x][y] > altitude[rp.x][rp.y])
					{
						//chance = 100 + altitude[rp.x][rp.y] - altitude[x][y];
						chance = 5;
					}
					else
					{ // Better chance for places we slope down to
						//chance = 150 + altitude[rp.x][rp.y] - altitude[x][y];
						chance = 20;
					}
					if (tiles[x][y].terrain->has_flag(WTF_WATER))
					{
						chance += 7;
					}
					chances.push_back(chance);
					total_chance += chance;
				}
			}
			if (chances.empty())
			{
				done = true;
			}
// Now pick from among those options.
			if (!done)
			{
				int index = rng(1, total_chance);
				bool pick_done = false;
				for (int i = 0; !pick_done && i < chances.size(); i++)
				{
					index -= chances[i];
					if (index <= 0)
					{
						rp = next[i];
						pick_done = true;
					}
				}
			}
		}
	}

// Take everything with altitude <= 0 and set it to be ocean.
	for (int x = 0; x < WORLDMAP_SIZE; x++)
	{
		for (int y = 0; y < WORLDMAP_SIZE; y++)
		{
			if (altitude[x][y] <= 0 && !biomes[x][y]->has_flag(BIOME_FLAG_NO_OCEAN))
			{
				tiles[x][y].set_terrain("ocean");
				biomes[x][y] = BIOMES.lookup_name("ocean");
// If it's a hub, i.e. a city_seed, try to reposition it within 5 tiles
				if (city[x][y] == CITY_HUB)
				{
					for (int i = 0; i < city_seeds.size(); i++)
					{
						if (city_seeds[i].x == x && city_seeds[i].y == y)
						{
							i = city_seeds.size();
							bool done = false;
							for (int rad = 1; !done && rad <= 5; rad++)
							{
								for (int rx = x - rad; !done && rx <= x + rad; rx++)
								{
									int ry = y - rad;
									if (rx >= 0 && rx < WORLDMAP_SIZE &&
										ry >= 0 && ry < WORLDMAP_SIZE &&
										altitude[rx][ry] > 0 && city[rx][ry] != CITY_NOTCITY &&
										!done)
									{
										city[rx][ry] = CITY_RAW;
										city_seeds.push_back(Point(rx, ry));
										done = true;
									}
									ry = y + rad;
									if (rx >= 0 && rx < WORLDMAP_SIZE &&
										ry >= 0 && ry < WORLDMAP_SIZE &&
										altitude[rx][ry] > 0 && city[rx][ry] != CITY_NOTCITY &&
										!done)
									{
										city[rx][ry] = CITY_RAW;
										city_seeds.push_back(Point(rx, ry));
										done = true;
									}
								}
								for (int ry = y - rad; !done && ry <= y + rad; ry++)
								{
									int rx = x - rad;
									if (rx >= 0 && rx < WORLDMAP_SIZE &&
										ry >= 0 && ry < WORLDMAP_SIZE &&
										altitude[rx][ry] > 0 && city[rx][ry] != CITY_NOTCITY &&
										!done)
									{
										city[rx][ry] = CITY_RAW;
										city_seeds.push_back(Point(rx, ry));
										done = true;
									}
									rx = x + rad;
									if (rx >= 0 && rx < WORLDMAP_SIZE &&
										ry >= 0 && ry < WORLDMAP_SIZE &&
										altitude[rx][ry] > 0 && city[rx][ry] != CITY_NOTCITY &&
										!done)
									{
										city[rx][ry] = CITY_RAW;
										city_seeds.push_back(Point(rx, ry));
										done = true;
									}
								}
							}
						}
					}
				}
				city[x][y] = CITY_NOTCITY;
			}
			else
			{
				if (city[x][y] == CITY_HUB)
				{
					city[x][y] = CITY_RAW;
				}
				int range = tiles[x][y].terrain->beach_range;
				if (range != -1)
				{
					for (int xn = x - range; xn <= x + range; xn++)
					{
						for (int yn = y - range; yn <= y + range; yn++)
						{
							if (xn >= 0 && xn < WORLDMAP_SIZE &&
								yn >= 0 && yn < WORLDMAP_SIZE && altitude[xn][yn] <= 0)
							{
								tiles[x][y].terrain = make_into_beach(tiles[x][y].terrain);
							}
						}
					}
				}
			}
		}
	}

// Erase any remaining below-water cities
	for (int i = 0; i < city_seeds.size(); i++)
	{
		Point p = city_seeds[i];
		if (tiles[p.x][p.y].terrain->road_cost <= 0 ||
			altitude[p.x][p.y] <= 0)
		{
			city_seeds.erase(city_seeds.begin() + i);
			i--;
		}
	}
// Draw some roads between cities.
	if (city_seeds.size() > 1)
	{
		for (int i = 0; i < city_seeds.size(); i++)
		{
			Generic_map gmap = get_generic_map();
			Pathfinder pf(gmap);
			pf.set_allow_diagonal(false);
			pf.set_bounds(20);
			Point from = city_seeds[i];
// This is a roll to get any index EXCEPT the current one;
// If we roll the current one, use the last one (which the roll skips)
			int index = rng(0, city_seeds.size() - 2);
			if (index == i)
			{
				index = city_seeds.size() - 1;
			}
			Point to = city_seeds[index];

			Path path = pf.get_path(PATH_A_STAR, from, to);
			if (path.get_cost() <= 150000)
			{
				for (int n = 0; n < path.size(); n++)
				{
					Tripoint p = path[n];
					if (!tiles[p.x][p.y].terrain->has_flag(WTF_NO_ROAD))
					{
						if (tiles[p.x][p.y].terrain->has_flag(WTF_BRIDGE))
						{
							tiles[p.x][p.y].set_terrain("bridge");
						}
						else
						{
							tiles[p.x][p.y].set_terrain("road");
							if (city[p.x][p.y] != CITY_NOTCITY)
							{
								city[p.x][p.y] = CITY_ROAD;
							}
						}
					}
				}
			}
		}
	}

// Now fill in cities!
	for (int i = 0; i < city_seeds.size(); i++)
	{
		std::vector<Point> active;
		active.push_back(city_seeds[i]);
		bool vertical_blocks = one_in(2);
		int block_size = rng(4, 8);

		while (!active.empty())
		{
			int index = 0;
			Point p = active[index];
			if ((vertical_blocks && (p.x % 3 != 0 && p.y % block_size != 0)) ||
				(!vertical_blocks && (p.y % 3 != 0 && p.x % block_size != 0)))
			{
				index = rng(0, active.size() - 1);
				p = active[index];
			}
			active.erase(active.begin() + index);

			bool expansions = false;
			int roads = 0;
			City_status stat = city[p.x][p.y];
			for (int n = 0; n < 4; n++)
			{
				Point expand;
				switch (n)
				{
				case 0:
					expand = Point(p.x - 1, p.y);
					break;
				case 1:
					expand = Point(p.x + 1, p.y);
					break;
				case 2:
					expand = Point(p.x, p.y - 1);
					break;
				case 3:
					expand = Point(p.x, p.y + 1);
					break;
				}
				if (expand.x >= 0 && expand.x < WORLDMAP_SIZE &&
					expand.y >= 0 && expand.y < WORLDMAP_SIZE)
				{

					if (city[expand.x][expand.y] == CITY_RAW &&
						stat != CITY_BUILDING_CLOSED)
					{

						if ((vertical_blocks &&
							 (expand.x % 3 == 0 || expand.y % block_size == 0)) ||
							(!vertical_blocks &&
							 (expand.y % 3 == 0 || expand.x % block_size == 0)))
						{
							active.insert(active.begin(), expand);
						}
						else
						{
							active.push_back(expand);
						}

						city[expand.x][expand.y] = CITY_BUILDING;
// TODO: Set the distance range based on the city's size?
						if (rl_dist(expand, city_seeds[i]) <= rng(3, 10) ||
							one_in(20))
						{
							tiles[expand.x][expand.y].terrain = random_shop();
						}
						else
						{
							tiles[expand.x][expand.y].set_terrain("house");
						}
						expansions = true;

					}
					else if (city[expand.x][expand.y] == CITY_ROAD)
					{
						if (stat != CITY_BUILDING_CLOSED)
						{
							active.push_back(expand);
						}
						roads++;

					}
					else if (city[expand.x][expand.y] == CITY_ROAD_CLOSED)
					{
						roads++;

					}
					else if (city[expand.x][expand.y] == CITY_BUILDING &&
							 stat != CITY_BUILDING_CLOSED)
					{
						city[expand.x][expand.y] = CITY_BUILDING_CLOSED;
					}

				}

				if (!expansions)
				{
					bool block_closer = ((vertical_blocks && p.x % 3 == 0) ||
										 (!vertical_blocks && p.y % 3 == 0));
					if (block_closer)
					{
						if (vertical_blocks && p.y % block_size != 1 &&
							p.y % block_size != block_size - 1)
						{
							block_closer = false;
						}
						else if (!vertical_blocks && p.x % block_size != 1 &&
								 p.x % block_size != block_size - 1)
						{
							block_closer = false;
						}
					}
					if (roads == 2 && block_closer)
					{
						city[p.x][p.y] = CITY_ROAD_CLOSED;
						tiles[p.x][p.y].set_terrain("road");
					}
					else if (city[p.x][p.y] == CITY_BUILDING)
					{
						city[p.x][p.y] = CITY_BUILDING_CLOSED;
					}
					else if (city[p.x][p.y] == CITY_ROAD)
					{
						city[p.x][p.y] = CITY_ROAD_CLOSED;
					}
				}
				else
				{
					city[p.x][p.y] = CITY_ROAD_CLOSED;
					tiles[p.x][p.y].set_terrain("road");
				}
			}
		}
	}

// Take any usued city tiles and make them field
	for (int x = 0; x < WORLDMAP_SIZE; x++)
	{
		for (int y = 0; y < WORLDMAP_SIZE; y++)
		{
			if (city[x][y] == CITY_RAW)
			{
				tiles[x][y].set_terrain("field");
				biomes[x][y] = BIOMES.lookup_name("grassland");
				int range = tiles[x][y].terrain->beach_range;
				for (int xn = x - range; xn <= x + range; xn++)
				{
					for (int yn = y - range; yn <= y + range; yn++)
					{
						if (xn >= 0 && xn < WORLDMAP_SIZE &&
							yn >= 0 && yn < WORLDMAP_SIZE && altitude[xn][yn] <= 0)
						{
							tiles[x][y].terrain = make_into_beach(tiles[x][y].terrain);
						}
					}
				}
			}
		}
	}
// Add some bonuses / road bonuses
// See worldmap.h for BONUS_SPACING definition
	for (int x = BONUS_SPACING; x < WORLDMAP_SIZE; x += BONUS_SPACING)
	{
		for (int y = BONUS_SPACING; y < WORLDMAP_SIZE; y += BONUS_SPACING)
		{
			if (!one_in(3))
			{
				add_bonus(x, y);
			}
		}
	}
// Finally, place mosnters!
	place_monsters();
}

void Worldmap::place_monsters()
{
	for (int x = 0; x < WORLDMAP_SIZE; x++)
	{
		for (int y = 0; y < WORLDMAP_SIZE; y++)
		{
			if (biomes[x][y])
			{
				Variable_monster_genus var = biomes[x][y]->monsters;
// Decide how many genera to place here.
				int placed = var.pick_number();
				if (placed > 0)
				{
					std::vector<Monster_genus*> gens = var.pick(placed);
					for (int i = 0; i < gens.size(); i++)
					{
						Monster_genus* genus = gens[i];
						int population = biomes[x][y]->monster_population.roll() / placed;
						if (genus && population > 0)
						{
							Monster_spawn tmpspawn;
							tmpspawn.genus = genus;
							tmpspawn.population = population;
							tiles[x][y].monsters.push_back(tmpspawn);
						}
					}
				}
			}
		}
	}
}

void Worldmap::add_bonus(int x, int y, bool debug)
{
// Check if we can place a road bonus - they're preferred
	std::vector<Point> road_spots;
	for (int rx = x - BONUS_SPACING / 2;
		 rx <= x + BONUS_SPACING / 2 && rx < WORLDMAP_SIZE - 1;
		 rx++)
	{
		for (int ry = y - BONUS_SPACING / 2;
			 ry <= y + BONUS_SPACING / 2 && ry < WORLDMAP_SIZE - 1;
			 ry++)
		{
			if (has_flag(WTF_ROAD, rx, ry))
			{
// It's a road; add all adjacent non-road tiles to the list
				for (int cx = rx - 1; cx <= rx + 1; cx++)
				{
					for (int cy = ry - 1; cy <= ry + 1; cy++)
					{
						if (!has_flag(WTF_ROAD, cx, cy))
						{
							road_spots.push_back(Point(cx, cy));
						}
					}
				}
			}
		}
	}
	int bx, by; // The actual spot we'll place our bonus
	bool road = false;
	if (road_spots.empty())
	{
		bx = x + rng(0 - BONUS_SPACING / 2, BONUS_SPACING / 2);
		by = y + rng(0 - BONUS_SPACING / 2, BONUS_SPACING / 2);
		if (debug)
		{
			debugmsg("No road; [%d:%d] => [%d:%d]", x, y, bx, by);
		}
	}
	else
	{
		road = true;
		int index = rng(0, road_spots.size() - 1);
		bx = road_spots[index].x;
		by = road_spots[index].y;
		if (debug)
		{
			debugmsg("%d valid road spots; [%d:%d] => [%d:%d]",
					road_spots.size(), bx, by);
		}
	}
	if (bx < 0)
	{
		bx = 0;
	}
	else if (bx >= WORLDMAP_SIZE)
	{
		bx = WORLDMAP_SIZE - 1;
	}
	if (by < 0)
	{
		by = 0;
	}
	else if (by >= WORLDMAP_SIZE)
	{
		by = WORLDMAP_SIZE - 1;
	}
// We've got our spot!  But what biome is it in...?
	Biome* bonus_biome = biomes[bx][by];
	World_terrain* bonus_ter = NULL;

	if (bonus_biome)
	{
		if (road)
		{
			bonus_ter = bonus_biome->pick_road_bonus();
		}
		else
		{
			bonus_ter = bonus_biome->pick_bonus();
		}

		if (bonus_ter)
		{
			if (debug)
			{
				debugmsg("Picked '%s'", bonus_ter->get_name().c_str());
			}
// Yay!  We're actually placing a real bonus.  So put it on the chosen tile...
			tiles[bx][by].terrain = bonus_ter;
// ... and if it spreads, maybe some adjacent tiles?
			int spread_points = bonus_ter->spread.roll();
			int surrounding = 0;  // For WTF_NO_SURROUND
			if (bonus_ter->spread_type == SPREAD_NULL)
			{
// TODO: Warn about this?  Shouldn't ever actually happen.
				spread_points = 0;
			}
			std::vector<Point> included;
			included.push_back(Point(bx, by));
			while (spread_points > 0)
			{
				if (debug)
				{
					debugmsg("Spread points: %d", spread_points);
				}
				std::vector<Point> spread_targets;
				std::vector<Point> spread_preferred;
				switch (bonus_ter->spread_type)
				{

/* Yes, the below makes it more likely to spread to points adjacent to more than
 * one already-used point.  Not only is this easier to code, but it's also often
 * more desirable behavior, since it leads to centralized blobs rather than
 * stretched-out things.
 */
				case SPREAD_NORMAL:
					for (int i = 0; i < included.size(); i++)
					{
						for (int x = included[i].x - 1; x <= included[i].x + 1; x++)
						{
							for (int y = included[i].y - 1; y <= included[i].y + 1; y++)
							{
// Don't use diagonally-adjacent tiles
								if (x == included[i].x || y == included[i].y)
								{
									Worldmap_tile* target = get_tile(x, y);
									if (target &&
										target->terrain->spread_cost <= spread_points &&
										(surrounding < 3 ||
										 !is_adjacent_no_diag(bx, by, x, y)))
									{
										spread_targets.push_back(Point(x, y));
									}
								}
							}
						}
					}
					break;

				case SPREAD_CENTER:
// By stopping as soon as we have targets, we always use the earliest point
					for (int i = 0; spread_targets.empty() && i < included.size(); i++)
					{
						for (int x = included[i].x - 1; x <= included[i].x + 1; x++)
						{
							for (int y = included[i].y - 1; y <= included[i].y + 1; y++)
							{
// Don't use diagonally-adjacent tiles
								if (x == included[i].x || y == included[i].y)
								{
									Worldmap_tile* target = get_tile(x, y);
									if (target &&
										target->terrain->spread_cost <= spread_points &&
										(surrounding < 3 ||
										 !is_adjacent_no_diag(bx, by, x, y)))
									{
										spread_targets.push_back(Point(x, y));
									}
								}
							}
						}
					}
					break;

				case SPREAD_ARMS:
					for (int i = 0; i < included.size(); i++)
					{
						for (int x = included[i].x - 1; x <= included[i].x + 1; x++)
						{
							for (int y = included[i].y - 1; y <= included[i].y + 1; y++)
							{
// Don't use diagonally-adjacent tiles
								if (x == included[i].x || y == included[i].y)
								{
									Worldmap_tile* target = get_tile(x, y);
									if (target &&
										target->terrain->spread_cost <= spread_points &&
										(surrounding < 3 ||
										 !is_adjacent_no_diag(bx, by, x, y)))
									{
// Check if we would create an "arm"
										bool arm = true;
										for (int ax = x - 1; arm && ax <= x + 1; ax++)
										{
											for (int ay = y - 1; arm && ay <= y + 1; ay++)
											{
												if (ay == y)
												{
													ay++; // Skip our own tile
												}
												for (int n = 0; arm && n < included.size(); n++)
												{
													if (included[n].x == ax && included[n].y == ay)
													{
														arm = false;
													}
												}
											}
										}
										if (arm)
										{  // This is a preferred point!
											spread_preferred.push_back(Point(x, y));
										}
										else
										{
											spread_targets.push_back(Point(x, y));
										}
									}
								}
							}
						}
					}
					break;

				} // switch (bonus_ter->spread_type)

				if (!spread_preferred.empty())
				{
// Replace full target list with only-preferred target list
					spread_targets = spread_preferred;
				}
				if (spread_targets.empty())
				{ // Nowhere to go, so sad :(
					spread_points = 0;
					if (debug)
					{
						debugmsg("No valid spread targets.");
					}
				}
				else
				{
					Point spread = spread_targets[rng(0, spread_targets.size() - 1)];
// For WTF_NO_SURROUND
					if (is_adjacent_no_diag(Point(bx, by), spread))
					{
						surrounding++;
					}
					included.push_back(spread);
					Worldmap_tile* target = get_tile(spread);
					spread_points -= target->terrain->spread_cost;
// If bonus_ter has spread options, pick one; otherwise, keep the terrain
					if (bonus_ter->spread_options.empty())
					{
						target->terrain = bonus_ter;
						if (debug)
						{
							debugmsg("No spread options; spreading '%s' to [%d:%d]",
									bonus_ter->get_name().c_str(), spread.x, spread.y);
						}
					}
					else
					{
						Variable_world_terrain options;
						std::istringstream options_ss(bonus_ter->spread_options);
						if (options.load_data(options_ss, bonus_ter->get_name()))
						{
							World_terrain* new_bonus = options.pick();
							if (new_bonus)
							{
								target->terrain = new_bonus;
								if (debug)
								{
									debugmsg("Spread chose '%s' to [%d:%d]",
											new_bonus->get_name().c_str(), spread.x, spread.y);
								}
							}
							else
							{
								if (debug)
								{
									debugmsg("Spread failed! Spreading '%s' to [%d:%d]",
											bonus_ter->get_name().c_str(), spread.x, spread.y);
								}
								target->terrain = bonus_ter;
							}
						}
					}
				}
			} // while (spread_points > 0)
		}
		else
		{ // if (bonus_ter)
			if (debug)
			{
				debugmsg("No bonuses for biome '%s'", bonus_biome->get_name().c_str());
			}
		}
	}
	else
	{ // if (bonus_biome)
		if (debug)
		{
			debugmsg("No biome there!");
		}
	}
}

void Worldmap::draw_island(std::vector<std::vector<int> >& altitude,
		Point center, int height, int edge_dist, int id)
{
	if (center.x < 0 || center.x >= WORLDMAP_SIZE ||
		center.y < 0 || center.y >= WORLDMAP_SIZE)
	{
		return;
	}
	altitude[center.x][center.y] = height;
	add_point_to_island(center, id);

	std::vector<Point> points_active;
	points_active.push_back(center);

	int center_point = WORLDMAP_SIZE / 2;
	int shift = WORLDMAP_SIZE / 10;

// As long as we have active points...
	while (!points_active.empty())
	{

// We're going to empty out points_active and replace it with new_points.
		std::vector<Point> new_points;
		while (!points_active.empty())
		{
// Pick a random point...
			int index = rng(0, points_active.size() - 1);
			Point p = points_active[index];
// Cycle through all adjacent points (no diagonals!)
			for (int i = 0; i < 4; i++)
			{
				int x, y;
				switch (i)
				{
				case 0:
					x = p.x - 1;
					y = p.y;
					break;
				case 1:
					x = p.x + 1;
					y = p.y;
					break;
				case 2:
					x = p.x;
					y = p.y - 1;
					break;
				case 3:
					x = p.x;
					y = p.y + 1;
					break;
				}
				if (x > 0 && x < WORLDMAP_SIZE && y > 0 && y < WORLDMAP_SIZE &&
					altitude[x][y] == 0)
				{
// Figure out how far we are from the edge - either vertically or horizontally
					int dist_from_edge = (x > center_point ? WORLDMAP_SIZE - x : x);
					int y_dist = (y > center_point ? WORLDMAP_SIZE - y : y);
					if (y_dist < dist_from_edge)
					{
						dist_from_edge = y_dist;
					}
// Add that adjacent point to new_points & copy altitude
					new_points.push_back(Point(x, y));
					altitude[x][y] = altitude[p.x][p.y];

// Reduce the altitude in one of many ways:
					if (dist_from_edge < rng(0, edge_dist))
					{
// Slope down rapidly if we're close to the edge, to avoid straight lines
						altitude[x][y] -= rng(20, 100);
					}
					else if (one_in(30))
					{
// Rarely, slope down rapidly.  This promotes gulfs and fjords.
						altitude[x][y] -= rng(0, 100);
					}
					else if (!one_in(10))
					{
/* Otherwise, 90% of the time slope slowly; 10% don't slope.  The latter
 * promotes peninsulae.
 */
						altitude[x][y] -= rng(0, shift);
					}
// If the altitude winds up greater than 0, add us to the island ID
					if (altitude[x][y] > 0)
					{
						add_point_to_island(Point(x, y), id);
					}
				} // if (is in bounds & altitude == 0)
			} // for (int i = 0; i < 4; i++)
// Erase the point we were working on
			points_active.erase(points_active.begin() + index);
		} // while (!active_points.empty())
		points_active = new_points;
	} // while (!active_points.empty())

// Set anything < 0 to 0.
	for (int x = 0; x < WORLDMAP_SIZE; x++)
	{
		for (int y = 0; y < WORLDMAP_SIZE; y++)
		{
			if (altitude[x][y] < 0)
			{
				altitude[x][y] = 0;
			}
		}
	}
}

void Worldmap::add_point_to_island(Point p, int id)
{
	if (id < 0)
	{
		return;
	}

	if (islands.count(id) == 0)
	{
		std::vector<Point> tmp;
		tmp.push_back(p);
		islands[id] = tmp;
	}
	else
	{
		islands[id].push_back(p);
	}
}
