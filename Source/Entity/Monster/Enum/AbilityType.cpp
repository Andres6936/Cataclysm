// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Entity/Monster/Enum/AbilityType.hpp"
#include <Cataclysm/Util/String/String.hpp>

Monster_ability_type lookup_monster_ability_type(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 1; i < MON_ABILITY_MAX; i++)
	{
		Monster_ability_type ret = Monster_ability_type(i);
		if (no_caps(monster_ability_type_name(ret)) == name)
		{
			return ret;
		}
	}
	return MON_ABILITY_NULL;
}

std::string monster_ability_type_name(Monster_ability_type type)
{
	switch (type)
	{
	case MON_ABILITY_NULL:
		return "NULL";
	case MON_ABILITY_SUMMON:
		return "summon";
	case MON_ABILITY_SIGNAL:
		return "signal";
	case MON_ABILITY_TERRAIN:
		return "terrain";
	case MON_ABILITY_TELEPORT:
		return "teleport";
	case MON_ABILITY_FIELDS:
		return "fields";
	case MON_ABILITY_MAX:
		return "ERROR - MON_ABILITY_MAX";
	default:
		return "ERROR - Unnamed Monster_ability_type";
	}
	return "ERROR - Escaped monster_ability_type_name() switch";
}
