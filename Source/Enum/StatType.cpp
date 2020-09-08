// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Enum/StatType.hpp"
#include <Cataclysm/Util/String/stringfunc.h>

Stat_id lookup_stat_id(std::string name)
{
	name = no_caps(name);
	name = trim(name);
	for (int i = 0; i < STAT_MAX; i++)
	{
		Stat_id ret = Stat_id(i);
		if (no_caps(stat_id_short_name(ret)) == name ||
			no_caps(stat_id_name(ret)) == name)
		{
			return ret;
		}
	}
	return STAT_NULL;
}

std::string stat_id_name(Stat_id id)
{
	switch (id)
	{
	case STAT_NULL:
		return "NULL";
	case STAT_STRENGTH:
		return "strength";
	case STAT_DEXTERITY:
		return "dexterity";
	case STAT_INTELLIGENCE:
		return "intelligence";
	case STAT_PERCEPTION:
		return "perception";
	case STAT_MAX:
		return "BUG - STAT_MAX";
	default:
		return "BUG - Unnamed Stat_id";
	}
	return "BUG - Escaped stat_id_name() switch";
}

std::string stat_id_short_name(Stat_id id)
{
	switch (id)
	{
	case STAT_NULL:
		return "NULL";
	case STAT_STRENGTH:
		return "str";
	case STAT_DEXTERITY:
		return "dex";
	case STAT_INTELLIGENCE:
		return "int";
	case STAT_PERCEPTION:
		return "per";
	case STAT_MAX:
		return "BUG - STAT_MAX";
	default:
		return "BUG - Unnamed Stat_id";
	}
	return "BUG - Escaped stat_id_short_name() switch";
}


