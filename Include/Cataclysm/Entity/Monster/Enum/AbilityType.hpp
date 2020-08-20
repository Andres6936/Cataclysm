// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ABILITYTYPE_HPP
#define CATACLYSM_ABILITYTYPE_HPP

#include <string>

enum Monster_ability_type
{
	MON_ABILITY_NULL = 0, // nuffin
	MON_ABILITY_SUMMON,   // create one or more named monsters
	MON_ABILITY_SIGNAL,   // apply signal to nearby terrain
	MON_ABILITY_TERRAIN,  // Change (i.e. generate) nearby terrain
	MON_ABILITY_TELEPORT, // Instantly teleport.
	MON_ABILITY_FIELDS,   // Place fields around us.
	MON_ABILITY_STATUS,   // Impart status effect(s) on nearby entities.
	MON_ABILITY_MAX
};

Monster_ability_type lookup_monster_ability_type(std::string name);

std::string monster_ability_type_name(Monster_ability_type type);

#endif //CATACLYSM_ABILITYTYPE_HPP
