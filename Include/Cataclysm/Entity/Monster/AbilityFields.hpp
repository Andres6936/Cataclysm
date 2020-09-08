// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ABILITYFIELDS_HPP
#define CATACLYSM_ABILITYFIELDS_HPP

#include <Cataclysm/Util/String/var_string.h>
#include "Cataclysm/Entity/Monster/Ability.hpp"

struct Monster_ability_fields : public Monster_ability
{
	Monster_ability_fields();

	~Monster_ability_fields()
	{
	}

	virtual Monster_ability_type type()
	{
		return MON_ABILITY_FIELDS;
	}

	virtual bool handle_data(std::string ident, std::istream& data,
			std::string owner);

	virtual bool effect(std::shared_ptr<Monster> user);

	int range;  // Radius of affected tiles; defaults to 1
	bool affect_all_tiles;  // If true, all tiles affected; defaults to false
	bool affect_self; // If true, affect tile under monster; defaults to false
	Dice tiles_affected;  // How many tiles to hit; ignored if <affect_all_tiles>
	Variable_string field_type; // Type of field (maybe random)
	Dice duration;  // Duration of field(s) generated; if <0 rolled, no field made
};

#endif //CATACLYSM_ABILITYFIELDS_HPP
