// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Enum/DamageType.hpp"
#include <Cataclysm/Util/String/stringfunc.h>

Damage_type lookup_damage_type(std::string name)
{
	name = no_caps(name);
	for (int i = 0; i < DAMAGE_MAX; i++)
	{
		Damage_type ret = Damage_type(i);
		if (name == no_caps(damage_type_name(ret)))
		{
			return ret;
		}
	}
	return DAMAGE_NULL;
}

std::string damage_type_name(Damage_type type)
{
	switch (type)
	{
	case DAMAGE_NULL:
		return "NULL";
	case DAMAGE_BASH:
		return "bash";
	case DAMAGE_CUT:
		return "cut";
	case DAMAGE_PIERCE:
		return "pierce";
	case DAMAGE_FIRE:
		return "fire";
	case DAMAGE_MAX:
		return "BUG - DAMAGE_MAX";
	default:
		return "Unnamed Damage_type";
	}
	return "BUG - Escaped switch";
}

