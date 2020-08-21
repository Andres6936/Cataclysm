// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPECONTAINER_HPP
#define CATACLYSM_TYPECONTAINER_HPP


class Item_type_container : public Item_type
{
public:
	Item_type_container();

	~Item_type_container()
	{
	}

	virtual Item_class get_class()
	{
		return ITEM_CLASS_CONTAINER;
	}

	virtual Item_action default_action()
	{
		return IACT_UNLOAD;
	}

	virtual std::string get_property_description();

	virtual bool always_combines()
	{
		return false;
	}

	virtual bool handle_data(std::string ident, std::istream& data);

	int capacity; // Volume capacity, that is
	std::string preposition; // A plastic bottle <preposition> water, e.g. "of"
	bool use_article; // If true: "A plastic bottle <preposition> SOME water
};

#endif //CATACLYSM_TYPECONTAINER_HPP
