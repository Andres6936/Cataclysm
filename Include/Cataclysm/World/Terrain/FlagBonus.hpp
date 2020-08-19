// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_FLAGBONUS_HPP
#define CATACLYSM_FLAGBONUS_HPP

#include <string>
#include <sstream>

#include <Cataclysm/Enum/TerrainFlag.hpp>

// For signal handling.
struct Terrain_flag_bonus
{
	Terrain_flag_bonus(Terrain_flag _flag = TF_NULL, int _amount = 0) :
			flag(_flag), amount(_amount)
	{
	}

	bool load_data(std::istream& data, std::string owner_name);

	Terrain_flag flag;
	int amount;
};

#endif //CATACLYSM_FLAGBONUS_HPP
