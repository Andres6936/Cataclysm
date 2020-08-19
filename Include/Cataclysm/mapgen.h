#ifndef _MAPGEN_SPEC_H_
#define _MAPGEN_SPEC_H_

#include <vector>
#include <map>
#include <list>
#include <string>

#include "Cataclysm/item_type.h"
#include "Cataclysm/var_string.h"
#include <Cataclysm/World/Terrain/terrain.h>

// MAPGEN_SIZE must be a divisor of SUBMAP_SIZE (specified in map.h)!
// Also, changing MAPGEN_SIZE will break all of the already-written mapgen specs
#define MAPGEN_SIZE 25

struct World_terrain;

/* Mapgen_spec_pool looks very much like a datapool but has a few special
 * features, so sadly it must be its own class.
 */
struct Terrain_chance
{
	Terrain_chance(int C = 10, Terrain* T = NULL) : chance(C), terrain(T)
	{
	}

	int chance;
	Terrain* terrain;
};

// number is used in Item_list_area, but not in Item_area
struct Item_type_chance
{
	Item_type_chance(int C = 10, int N = 1, Item_type* I = NULL) :
			chance(C), number(N), item(I)
	{
	}

	int chance;
	int number;
	Item_type* item;
};

struct Variable_terrain
{
public:
	Variable_terrain();

	~Variable_terrain()
	{
	}

	void add_terrain(int chance, Terrain* terrain);

	void add_terrain(Terrain_chance terrain);

	bool load_data(std::istream& data, std::string name = "unknown",
			bool allow_nothing = false);

	void prepare(); // If it's a lock, then define Terrain* choice.
	Terrain* pick(bool refresh_choice = false);

// Returns true if any of the options are the specified terrain
	bool contains(std::string terrain_name);

	bool contains(Terrain* terrain);

	bool lock;
	std::vector<Terrain_chance> ter;

private:
	int total_chance;
	Terrain* choice;

};

// Item_group is just a named list of Item_type_chances.
// It allows for shorthand in mapgen specs.
struct Item_group
{
	Item_group();

	~Item_group()
	{
	}

	std::string name;
	int uid;
	std::vector<Item_type_chance> item_types;
	int total_chance;

	void assign_uid(int id);

	std::string get_data_name();

// TODO: Do we need a get_name()?
	bool load_data(std::istream& data);

// This JUST loads the items line; used e.g. in furniture
	bool load_item_data(std::istream& data, std::string owner_name = "Unknown");

	void add_item(int chance, Item_type* item_type);

	void add_item(int chance, int number, Item_type* item_type);

	void add_item(Item_type_chance item_type);

	std::vector<Item_type*> get_all_item_types();

	Item_type* pick_type();
};

struct Item_area
{
public:
	Item_area();

	~Item_area()
	{
	}

	void add_item(int chance, Item_type* item_type);

	void add_item(int chance, int number, Item_type* item_type);

	void add_item(Item_type_chance item_type);

	void set_group(Item_group* group);

	void clear_points();

	void add_point(int x, int y);

	bool load_data(std::istream& data, std::string name = "unknown");

// Functions used for item placement.
	void reset(); // Resets counters used when use_all_items is true.
	bool place_item();

	Item_type* pick_type(std::string owner = "unknown");

	Point pick_location();

/* The chance the number of items placed to be increased:
 * We start with placing 0 items.  With an overall_chance% chance, we increase
 * the number by one.  Repeat until we fail the chance.
 */
	int overall_chance;

/* If true, then ALL items listed must be placed.  This is where the "number"
 * property of Item_type_chance comes into play.
 */
	bool use_all_items;

private:
	std::vector<Item_type_chance> item_types;
	std::vector<Point> locations;
	int total_chance;

// Used for tracking use_all_items
	int all_index;
	int all_count;
};

struct Item_amount
{
	Item_amount(int C = 10, Item_type* I = NULL) :
			chance(C), item(I)
	{
		amount = Dice(0, 1, 1);
	}

	Dice amount;
	int chance;
	Item_type* item;
};

struct Item_amount_area
{
public:
	Item_amount_area();

	~Item_amount_area()
	{
	}

	void add_item(Dice number, int chance, Item_type* group);

	void add_item(Item_amount group);

	void clear_points();

	void add_point(int x, int y);

	bool load_data(std::istream& data, std::string name = "unknown");

// Functions used for item placement.
	Item_amount pick_item();

	Point pick_location();

private:
	std::vector<Item_amount> items;
	std::vector<Point> locations;
	int total_chance;
};

struct Item_group_amount
{
	Item_group_amount(int C = 10, Item_group* G = NULL) :
			chance(C), group(G)
	{
		amount = Dice(0, 1, 1);
	}

	Dice amount;
	int chance;
	Item_group* group;
};

struct Item_group_amount_area
{
public:
	Item_group_amount_area();

	~Item_group_amount_area()
	{
	}

	void add_group(Dice number, int chance, Item_group* group);

	void add_group(Item_group_amount group);

	void clear_points();

	void add_point(int x, int y);

	bool load_data(std::istream& data, std::string name = "unknown");

// Functions used for item placement.
	Item_group_amount pick_group();

	Point pick_location();

private:
	std::vector<Item_group_amount> groups;
	std::vector<Point> locations;
	int total_chance;
};

struct Subst_chance
{
	Subst_chance(int C = 10, char R = '.') : chance(C), result(R)
	{
	}

	int chance;
	char result;
};

struct Tile_substitution
{
	Tile_substitution();

	~Tile_substitution()
	{
	}

	void add_result(int chance, char result);

	void add_result(Subst_chance chance);

	void load_data(std::istream& data, std::string name = "unknown");

	void make_selection();

	char current_selection();

	std::vector<Subst_chance> chances;
private:
	int total_chance;
	char selected;
};

enum Mapgen_flag
{
	MAPFLAG_NULL = 0,
	MAPFLAG_AUTOSTAIRS, // if level > 0, automatically match stairs below
	MAPFLAG_NOROTATE,   // Don't rotate randomly (still rotates for neighbors etc)
	MAPFLAG_SEPARATE_FURNITURE, // Furniture is never more than 1 tile
	MAPFLAG_MAX
};

Mapgen_flag lookup_mapgen_flag(std::string name);

std::string mapgen_flag_name(Mapgen_flag flag);

struct Mapgen_spec
{
	Mapgen_spec();

	~Mapgen_spec()
	{
	}

	bool load_data(std::istream& data);

	void prepare(World_terrain* world_ter[5] = NULL, bool allow_rotation = true);

	void random_rotate();

	void rotate(Direction dir);

	std::string get_letter(int x, int y); // For debugging purposes
	Terrain* pick_terrain(int x, int y);

	Furniture_type* pick_furniture(int x, int y);

	int pick_furniture_uid(int x, int y);

	std::string get_name();

	std::string get_display_name();

	std::string get_short_name(); // Without the type included
	bool has_flag(Mapgen_flag flag);

	void debug_output();

	int uid;
	std::string name;
	std::string terrain_name; // World_terrain we belong to
	std::string adj_terrain_name; // For adjacency maps; only place here
// The following two are used for building 2nd floors.
	std::string subname;  // Specific flavor of the terrain type
	Direction rotation;   // Rotation of the floor below

	bool is_adjacent; // If true, this is instructions for building adjacent to
	// terrain_name
	int num_neighbors; // For relational maps, how many neighbors we have
	// Note that "11" means two neighbors on opposite sides
	int weight;      // The likelihood of using this spec
	int z_level;
	std::map<char, Variable_terrain> terrain_defs;
	std::map<char, Item_area> item_defs;
	std::map<char, Item_group_amount_area> item_group_defs;
	std::map<char, Item_amount_area> item_amount_defs;
	std::map<char, Tile_substitution> substitutions;
	std::map<char, Furniture_type*> furniture;
	std::list<std::string> shuffles;
	Variable_terrain base_terrain; // Default terrain

	char terrain[MAPGEN_SIZE][MAPGEN_SIZE]; // Keys to terrain_defs, item_defs etc
	char prepped_terrain[MAPGEN_SIZE][MAPGEN_SIZE]; // After prepare() (subst etc)

private:
	bool verify_map();

	void assign_furniture_uids();

	void mark_furniture_uid(int x, int y, int uid);  // Recurses into neighbors

	int furniture_uid[MAPGEN_SIZE][MAPGEN_SIZE];
	std::vector<bool> flags;

	Variable_string name_options; // Potential results for our display name
	std::string display_name; // Set in prepare() via name_options

};

class Mapgen_spec_pool
{
public:
	Mapgen_spec_pool();

	~Mapgen_spec_pool();

	bool load_from(std::string filename);

	bool load_element(std::istream& data);

	Mapgen_spec* lookup_uid(int uid);

	Mapgen_spec* lookup_name(std::string name);

	std::vector<Mapgen_spec*> lookup_terrain_name(std::string name);

	Mapgen_spec* random_for_terrain(std::string name);

	std::vector<Mapgen_spec*> lookup_terrain_ptr(World_terrain* ptr);

	Mapgen_spec* random_for_terrain(World_terrain* ptr);

	Mapgen_spec* random_for_terrain(World_terrain* ptr,
			std::vector<bool> neighbor);

	Mapgen_spec* random_for_terrain(World_terrain* ptr, std::string subname,
			int z_level);

	Mapgen_spec* random_with_subname(std::string subname, int z_level = 0);

	std::vector<Mapgen_spec*> lookup_adjacent_name(std::string name);

	Mapgen_spec* random_adjacent_to(std::string name, std::string here);

	std::vector<Mapgen_spec*> lookup_adjacent_ptr(World_terrain* ptr);

	Mapgen_spec* random_adjacent_to(World_terrain* ptr,
			World_terrain* ptr_here = NULL);

	int size();

	std::list<Mapgen_spec*> instances;
private:
	int next_uid;
	std::map<int, Mapgen_spec*> uid_map;
	std::map<std::string, Mapgen_spec*> name_map;

	std::map<std::string, std::vector<Mapgen_spec*> > terrain_name_map;
	std::map<std::string, int> terrain_name_total_chance;
	std::map<World_terrain*, std::vector<Mapgen_spec*> > terrain_ptr_map;
	std::map<World_terrain*, int> terrain_ptr_total_chance;

	std::map<std::string, std::vector<Mapgen_spec*> > adjacent_name_map;
	std::map<std::string, int> adjacent_name_total_chance;
	std::map<World_terrain*, std::vector<Mapgen_spec*> > adjacent_ptr_map;
	std::map<World_terrain*, int> adjacent_ptr_total_chance;

	std::map<std::string, std::vector<Mapgen_spec*> > subname_map;
	std::map<std::string, int> subname_total_chance;
};

#endif
