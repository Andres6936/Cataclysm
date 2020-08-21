// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPEFOOD_HPP
#define CATACLYSM_TYPEFOOD_HPP

#include <Cataclysm/status_effect.h>
#include "Cataclysm/Item/item_type.h"

class Item_type_food : public Item_type
{
public:
	Item_type_food();

	~Item_type_food()
	{
	};

	virtual Item_class get_class()
	{
		return ITEM_CLASS_FOOD;
	}

	virtual Item_action default_action()
	{
		return IACT_EAT;
	}

	virtual std::string get_property_description();

	virtual int default_charges()
	{
		return charges;
	}

	virtual bool uses_charges()
	{
		return (charges > 1);
	}

	virtual bool handle_data(std::string ident, std::istream& data);

// TODO: Add morale boost?

	int food;
	int water;
	int effect_chance;    // Chance to inflict the effect below
	Status_effect effect; // Special effect inflicted; see status_effect.h
	int charges;  // So that you can partially consume a food item
	std::string verb; // Verb for consuming this.  "You ____ your <name>"

};

#endif //CATACLYSM_TYPEFOOD_HPP
