// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPEAMMO_HPP
#define CATACLYSM_TYPEAMMO_HPP


#include "Cataclysm/Item/item_type.h"

class Item_type_ammo : public Item_type
{
public:
	Item_type_ammo();

	~Item_type_ammo()
	{
	}

	virtual Item_class get_class()
	{
		return ITEM_CLASS_AMMO;
	}

	virtual std::string get_name_singular();  // Slightly more verbose
	virtual std::string get_name_plural();

	virtual std::string get_property_description();

	virtual bool handle_data(std::string ident, std::istream& data);

	virtual int default_charges()
	{
		return count;
	}

	virtual bool combine_by_charges()
	{
		return true;
	}

	virtual bool uses_charges()
	{
		return true;
	}

	std::string ammo_type;  // Ammo type - links this to a launcher
	int damage;       // Base damage
	int armor_pierce; // Armor ignored
	int range;
	Dice accuracy;    // Low is good!  In 1/10ths of a degree
	int count;        // How many to a box
	int pellets;      // How many projectiles in a single shot?  Only for 00 shot.
};

#endif //CATACLYSM_TYPEAMMO_HPP
