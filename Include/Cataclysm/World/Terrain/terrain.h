#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <vector>
#include <map>
#include <list>

#include <Cataclysm/glyph.h>
#include "Cataclysm/World/Terrain/Smash.hpp"
#include "Cataclysm/World/Terrain/SignalHandler.hpp"

struct Terrain
{
	int uid;
	std::string name;
	std::string display_name;
	glyph sym;
	unsigned int movecost;
/* Height ranged from 0-100 and reflects how much vertical space is blocked (for
 * line of sight calculations).  It defaults to 100.
 */
	unsigned int height;
	unsigned int hp;      // Defaults to 0.  0 HP means it's indestructible.

	std::string inverse;  // For stairs - the opposite direction

	Terrain_smash smash;
	bool smashable;
	std::string destroy_result;
// A map of what happens when a tool's signal is applied
	std::map<std::string, Terrain_signal_handler> signal_handlers;

	bool can_smash()
	{
		return smashable;
	}

	Terrain();

	~Terrain()
	{
	}

	std::string get_data_name();

	std::string get_name();

	void assign_uid(int id)
	{
		uid = id;
	}

	bool load_data(std::istream& data);

	bool has_flag(Terrain_flag flag);

private:
	std::vector<bool> flags;
};

struct Item_group;

struct Furniture_type
{
	int uid;
	std::string name;
	std::string display_name;
	glyph sym;

	unsigned int move_cost;
	unsigned int height;
	unsigned int weight;
	unsigned int hp;

	Terrain_smash smash;
	bool smashable;
// Items dropped when we destroy it
	Item_group* components;

	Furniture_type();

	~Furniture_type();

	std::string get_data_name();

	std::string get_name();

	void assign_uid(int id);

	bool has_flag(Terrain_flag flag);

	bool load_data(std::istream& data);

private:
	std::vector<bool> flags;
/* If owns_components is true, then components was created by us, and should be
 * deleted in our destructor.
 * If it's false, then components is a pointer to something in ITEM_GROUPS and
 * should NOT be deleted in our destructor.
 * It defaults to true, since "components = new Item_group" is in our
 * constructor.  If we have a "preset_components:" line in our data file, then
 * we reference an Item_group in ITEM_GROUPS and set owns_components to false.
 */
	bool owns_components;
};

#endif
