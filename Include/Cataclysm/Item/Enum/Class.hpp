// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_CLASS_HPP
#define CATACLYSM_CLASS_HPP

#include <string>

/* After adding an Item_class, be sure to edit Item_type.cpp and add the class
 * to the function item_class_name().
 */
enum Item_class
{
	ITEM_CLASS_MISC = 0,
	ITEM_CLASS_CLOTHING,
	ITEM_CLASS_AMMO,
	ITEM_CLASS_LAUNCHER,
	ITEM_CLASS_FOOD,
	ITEM_CLASS_TOOL,
	ITEM_CLASS_BOOK,
	ITEM_CLASS_CONTAINER,
	ITEM_CLASS_CORPSE,
	ITEM_CLASS_MAX
};

Item_class lookup_item_class(std::string name);

std::string item_class_name_plural(Item_class iclass);

std::string item_class_name(Item_class iclass, bool plural = false);

#endif //CATACLYSM_CLASS_HPP
