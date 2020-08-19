// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SMASH_HPP
#define CATACLYSM_SMASH_HPP

#include <Cataclysm/dice.h>
#include <Cataclysm/Enum/DamageType.hpp>

struct Terrain_smash
{
	Terrain_smash();

	~Terrain_smash()
	{
	}

	std::string failure_sound;
	std::string success_sound;
	Dice armor[DAMAGE_MAX];
	int ignore_chance;

	bool load_data(std::istream& data, std::string name = "unknown");
};

#endif //CATACLYSM_SMASH_HPP
