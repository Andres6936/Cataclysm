// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Enum/ItemFlag.hpp"
#include <Cataclysm/Util/String/stringfunc.h>


Item_flag lookup_item_flag(std::string name)
{
	name = no_caps(name);
	for (int i = 0; i < ITEM_FLAG_MAX; i++)
	{
		Item_flag ret = Item_flag(i);
		if (no_caps(item_flag_name(ret)) == name)
		{
			return ret;
		}
	}
	return ITEM_FLAG_NULL;
}

std::string item_flag_name(Item_flag flag)
{
	switch (flag)
	{
	case ITEM_FLAG_NULL:
		return "NULL";
	case ITEM_FLAG_LIQUID:
		return "liquid";
	case ITEM_FLAG_FLAMMABLE:
		return "flammable";
	case ITEM_FLAG_PLURAL:
		return "plural";
	case ITEM_FLAG_CONSTANT:
		return "constant_volume_weight";
	case ITEM_FLAG_RELOAD_SINGLE:
		return "reload_single";
	case ITEM_FLAG_OPEN_END:
		return "open_end";
	case ITEM_FLAG_GLASSES:
		return "glasses";
	case ITEM_FLAG_RELOAD_STR:
		return "reload_strength";
	case ITEM_FLAG_MAX:
		return "BUG - ITEM_FLAG_MAX";
	default:
		return "BUG - Unnamed Item_flag";
	}
	return "BUG - Escaped item_flag_name switch";
}

