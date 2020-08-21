// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPECLOTHING_HPP
#define CATACLYSM_TYPECLOTHING_HPP

#include <Cataclysm/Enum/BodyPart.hpp>
#include "Cataclysm/Item/item_type.h"

class Item_type_clothing : public Item_type
{
public:
	Item_type_clothing();

	~Item_type_clothing()
	{
	}

	virtual Item_class get_class()
	{
		return ITEM_CLASS_CLOTHING;
	}

	virtual Item_action default_action()
	{
		return IACT_WEAR;
	}

	virtual std::string get_property_description();

	virtual bool handle_data(std::string ident, std::istream& data);

	int carry_capacity;
	int armor_bash;
	int armor_cut;
	int armor_pierce;
	int protection; // Protection from Status_effects that use this vector
	int encumbrance;
	bool covers[BODY_PART_MAX];

};

#endif //CATACLYSM_TYPECLOTHING_HPP
