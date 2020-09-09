// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_FURNITURETYPE_HPP
#define CATACLYSM_FURNITURETYPE_HPP

#include <string>
#include <vector>
#include <istream>
#include <cstdint>

#include <Cataclysm/Graphics/glyph.h>
#include <Cataclysm/Enum/TerrainFlag.hpp>
#include "Cataclysm/World/Terrain/Smash.hpp"

struct Item_group;

struct Furniture_type
{
	std::string name {"Error"};
	std::string display_name;
	glyph sym;

	bool smashable {false};

	std::int32_t uid {-1};

	std::uint32_t move_cost {100};
	std::uint32_t height {0};
	std::uint32_t weight {0};
	std::uint32_t hp {0};

	Terrain_smash smash;

	/**
	 * Items dropped when we destroy it.
	 */
	Item_group* components = new Item_group;

	Furniture_type();

	~Furniture_type();

	std::string get_data_name();

	std::string get_name();

	void assign_uid(int id);

	bool has_flag(Terrain_flag flag);

	bool load_data(std::istream& data);

private:

	std::vector<bool> flags;

	/**
	 * If owns_components is true, then components was created by us,
	 * and should be deleted in our destructor.
	 *
	 * If it's false, then components is a pointer to something in
	 * ITEM_GROUPS and should NOT be deleted in our destructor.
	 *
	 * It defaults to true, since "components = new Item_group" is
	 * in our constructor.  If we have a "preset_components:" line
	 * in our data file, then we reference an Item_group in ITEM_GROUPS
	 * and set owns_components to false.
	 */
	bool owns_components {true};
};

#endif //CATACLYSM_FURNITURETYPE_HPP
