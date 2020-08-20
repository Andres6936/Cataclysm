// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ABILITYTERRAIN_HPP
#define CATACLYSM_ABILITYTERRAIN_HPP

#include <Cataclysm/dice.h>
#include <Cataclysm/var_string.h>
#include "Cataclysm/Entity/Monster/Ability.hpp"

struct Monster_ability_terrain : public Monster_ability
{
	Monster_ability_terrain();

	~Monster_ability_terrain()
	{
	}

	virtual Monster_ability_type type()
	{
		return MON_ABILITY_TERRAIN;
	}

	virtual bool handle_data(std::string ident, std::istream& data,
			std::string owner);

	virtual bool effect(Monster* user);

	bool always_replace;  // Defaults to false; if true, always replace terrain

/* Damage is only used if always_replace is false (it defaults to false).  The
 * targeted terrain will be automatically replaced anyway if its HP is 0;
 * otherwise, we reduce its HP by a roll of <damage> and if it reaches 0 (or
 * less), then we do the replacement.
 * Note that by setting <damage> but leaving <terrain> empty, this ability is
 * effectively a terrain-damaging ability (destroyed terrain will be replaced by
 * its <destroy_result>).
 */
	Dice damage;
	Variable_string terrain;  // So that it can be randomized
	int range;  // Radius of ability; defaults to 1
	Dice tiles_affected;  // How many tiles to affect; defaults to 1
};

#endif //CATACLYSM_ABILITYTERRAIN_HPP
