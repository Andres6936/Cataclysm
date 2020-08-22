#ifndef _BIOME_H_
#define _BIOME_H_

#include <string>
#include <vector>
#include <Cataclysm/World/VariableTerrain.hpp>
#include <Cataclysm/World/Biome/Enum/BiomeFlag.hpp>
#include "Cataclysm/World/Biome/VariableMonsterGenus.hpp"

struct Biome
{
	Biome();

	~Biome();

	std::string name;
	std::string display_name;
	int uid;

	Variable_world_terrain terrain;
	Variable_world_terrain bonuses;
	Variable_world_terrain road_bonuses;

	Variable_monster_genus monsters;
	Dice monster_population;

	void assign_uid(int id);

	std::string get_data_name();

	std::string get_name();

	bool load_data(std::istream& data);

	World_terrain* pick_terrain();

	World_terrain* pick_bonus();

	World_terrain* pick_road_bonus();

	bool has_flag(Biome_flag flag);

private:
	std::vector<bool> flags;
};

#endif
