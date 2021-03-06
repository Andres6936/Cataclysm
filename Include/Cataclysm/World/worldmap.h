#ifndef _WORLDMAP_H_
#define _WORLDMAP_H_

#include <map>
#include <string>

#include <Cuss/Element.hpp>
#include <Cataclysm/Util/pathfind.h>
#include "Cataclysm/World/Tile.hpp"
#include <Cataclysm/World/world_terrain.h>
#include <Cataclysm/World/Biome/biome.h>
#include <Cataclysm/Entity/Monster/monster_spawn.h>

#define WORLDMAP_SIZE 150
#define BONUS_SPACING   8

class Worldmap
{
public:
	Worldmap();

	~Worldmap();

	std::string get_name();

	void set_name(std::string N);

	void randomize_name();

	void generate();

	void place_monsters();

	void add_bonus(int x, int y, bool debug = false);

	void set_terrain(int x, int y, std::string terrain_name);

	void init_shop_picker();

	World_terrain* random_shop();

	Point get_point(int posx, int posy);

	std::vector<Point> find_terrain(std::string name,
			Point origin = Point(-1, -1), int range = -1);

	void draw_minimap(cuss::element* drawing, int cornerx, int cornery);

	Worldmap_tile* get_tile(int x, int y, bool warn = false);

	Worldmap_tile* get_tile(Point p, bool warn = false);

	glyph get_glyph(int x, int y);

	std::string get_name(int x, int y);

	std::vector<Monster_spawn>* get_spawns(int x, int y);

	bool has_flag(World_terrain_flag flag, int x, int y);

	Generic_map get_generic_map();  // For road drawing and long-travel

	Point random_tile_with_terrain(std::string name, int island = -1);

	Point random_tile_with_terrain(World_terrain* terrain, int island = -1);

	bool save_to_name();

	std::string save_data();

	bool load_from_file(std::string filename);

	bool load_data(std::istream& data);

private:
	void draw_island(std::vector<std::vector<int> >& altitude,
			Point center, int height, int edge_dist, int id);

	void add_point_to_island(Point p, int id);

	Worldmap_tile tiles[WORLDMAP_SIZE][WORLDMAP_SIZE];
	Biome* biomes[WORLDMAP_SIZE][WORLDMAP_SIZE];

	Worldmap_tile tile_oob;

	std::map<int, std::vector<Point> > islands;
// shops and shop_count are ONLY used in generation
	std::vector<World_terrain*> shops;
	std::map<World_terrain*, int> shop_count;

	std::string name;
};

inline std::unique_ptr<Worldmap> worldmap {nullptr};

#endif
