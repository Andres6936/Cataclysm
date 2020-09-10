// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Enum/SpreadType.hpp"
#include <Cataclysm/Util/String/String.hpp>

Spread_type lookup_spread_type(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 0; i < SPREAD_MAX; i++)
	{
		Spread_type ret = Spread_type(i);
		if (no_caps(spread_type_name(ret)) == name)
		{
			return ret;
		}
	}
	return SPREAD_NULL;
}

std::string spread_type_name(Spread_type type)
{
	switch (type)
	{
	case SPREAD_NULL:
		return "NULL";
	case SPREAD_NORMAL:
		return "normal";
	case SPREAD_CENTER:
		return "center";
	case SPREAD_ARMS:
		return "arms";
	case SPREAD_MAX:
		return "BUG - SPREAD_MAX";
	default:
		return "BUG - Unnamed Spread_type";
	}
	return "BUG - Escaped spread_type_name() switch";
}


