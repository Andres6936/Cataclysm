#include "Cataclysm/Entity/Monster/monster_type.h"
#include "Cataclysm/stringfunc.h"

Monster_size lookup_monster_size(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 0; i < MON_SIZE_MAX; i++)
	{
		Monster_size ret = Monster_size(i);
		if (no_caps(monster_size_name(ret)) == name)
		{
			return ret;
		}
	}
	return MON_SIZE_NULL;
}

std::string monster_size_name(Monster_size size)
{
	switch (size)
	{
	case MON_SIZE_NULL:
		return "NULL";
	case MON_SIZE_TINY:
		return "tiny";
	case MON_SIZE_SMALL:
		return "small";
	case MON_SIZE_MEDIUM:
		return "medium";
	case MON_SIZE_LARGE:
		return "large";
	case MON_SIZE_HUGE:
		return "huge";
	case MON_SIZE_MAX:
		return "BUG - MON_SIZE_MAX";
	default:
		return "BUG - Unnamed Monster_size";
	}
	return "BUG - Escaped monster_size_name switch";
}
