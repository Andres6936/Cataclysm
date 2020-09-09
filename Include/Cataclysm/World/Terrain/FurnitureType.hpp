// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_FURNITURETYPE_HPP
#define CATACLYSM_FURNITURETYPE_HPP

#include <string>
#include <vector>
#include <istream>
#include <Cataclysm/Graphics/glyph.h>
#include <Cataclysm/Enum/TerrainFlag.hpp>
#include "Cataclysm/World/Terrain/Smash.hpp"

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

#endif //CATACLYSM_FURNITURETYPE_HPP
