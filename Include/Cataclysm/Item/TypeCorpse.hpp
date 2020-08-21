// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPECORPSE_HPP
#define CATACLYSM_TYPECORPSE_HPP

class Item_type_corpse : public Item_type
{
public:
	Item_type_corpse();

	~Item_type_corpse()
	{
	}

	virtual Item_class get_class()
	{
		return ITEM_CLASS_CORPSE;
	}

	virtual bool handle_data(std::string ident, std::istream& data);
};

#endif //CATACLYSM_TYPECORPSE_HPP
