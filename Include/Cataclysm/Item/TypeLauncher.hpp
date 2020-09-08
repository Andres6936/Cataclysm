// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPELAUNCHER_HPP
#define CATACLYSM_TYPELAUNCHER_HPP

#include <Cataclysm/Entity/skill.h>
#include "Cataclysm/Item/item_type.h"

class Item_type_launcher : public Item_type
{
public:
	Item_type_launcher();

	~Item_type_launcher()
	{
	}

	virtual Item_class get_class()
	{
		return ITEM_CLASS_LAUNCHER;
	}

	virtual bool handle_data(std::string ident, std::istream& data);

	virtual std::string get_property_description();

	virtual int time_to_reload()
	{
		return reload_ap;
	}

	virtual int time_to_fire()
	{
		return fire_ap;
	}

	virtual bool uses_charges()
	{
		return true;
	}

// TODO: Add in gun class / skill used / however we do that
	std::string ammo_type;  // Ammo type - links this to a launcher
	int damage;     // Damage bonus
	Dice accuracy;   // Low is good!  In 1/10ths of a degree
	int recoil;     // Recoil added
	int durability; // HP basically
	int capacity;   // Shots per reload
	int reload_ap;  // action_points per reload
	int fire_ap;    // action_points per shot fired
	Skill_type skill_used;  // Handguns, shotguns, SMGs, or rifles
	std::vector<int> modes; // Each element is a number of shots

};

#endif //CATACLYSM_TYPELAUNCHER_HPP
