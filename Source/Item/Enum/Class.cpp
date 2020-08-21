// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Item/Enum/Class.hpp"


Item_class lookup_item_class(std::string name)
{
	name = no_caps(name);
	for (int i = 0; i < ITEM_CLASS_MAX; i++)
	{
		Item_class ret = Item_class(i);
		if (no_caps(item_class_name(ret)) == name)
		{
			return ret;
		}
	}
	return ITEM_CLASS_MISC;
}

std::string item_class_name_plural(Item_class iclass)
{
	return item_class_name(iclass, true);
}

// plural defaults to false
std::string item_class_name(Item_class iclass, bool plural)
{
	switch (iclass)
	{
	case ITEM_CLASS_MISC:
		return "Misc";
	case ITEM_CLASS_CLOTHING:
		return "Clothing";
	case ITEM_CLASS_AMMO:
		return "Ammo";
	case ITEM_CLASS_LAUNCHER:
		return (plural ? "Launchers" : "Launcher");
	case ITEM_CLASS_FOOD:
		return "Food";
	case ITEM_CLASS_TOOL:
		return (plural ? "Tools" : "Tool");
	case ITEM_CLASS_BOOK:
		return (plural ? "Books" : "Book");
	case ITEM_CLASS_CONTAINER:
		return (plural ? "Containers" : "Container");
	case ITEM_CLASS_CORPSE:
		return (plural ? "Corpses" : "Corpse");

	case ITEM_CLASS_MAX:
		return "BUG - ITEM_CLASS_MAX";
	default:
		return "BUG - Unnamed Item_class";
	}
	return "BUG - Escaped item_class_name switch";
}

