// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_DAMAGETYPE_HPP
#define CATACLYSM_DAMAGETYPE_HPP

#include <string>

// TODO: Fire damage? Acid damage? etc etc
enum Damage_type
{
	DAMAGE_NULL = 0,
	DAMAGE_BASH,
	DAMAGE_CUT,
	DAMAGE_PIERCE,
	DAMAGE_FIRE,
	DAMAGE_MAX
};

Damage_type lookup_damage_type(std::string name);

std::string damage_type_name(Damage_type type);

#endif //CATACLYSM_DAMAGETYPE_HPP
