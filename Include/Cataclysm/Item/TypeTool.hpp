// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPETOOL_HPP
#define CATACLYSM_TYPETOOL_HPP


class Item_type_tool : public Item_type
{
public:
	Item_type_tool();

	~Item_type_tool()
	{
	};

	virtual Item_class get_class()
	{
		return ITEM_CLASS_TOOL;
	}

	virtual Item_action default_action()
	{
		return IACT_APPLY;
	}

	virtual int default_charges()
	{
		return def_charges;
	}

	virtual std::string get_property_description();

	virtual bool handle_data(std::string ident, std::istream& data);

	virtual bool uses_charges(); // true if max_charges > 0 && charges_per_use > 0
	virtual bool always_combines()
	{
		return false;
	}

	Tool_action applied_action;   // Action when applied
	Tool_action powered_action;   // Action every turn, while powered
	Tool_action countdown_action; // Action when countdown finishes

	int def_charges;  // Charges it starts with
	int max_charges;      // Max charges.  If 0, doesn't use charges.
	int subcharges;       // How many turns, while powered, until we use 1 charge
	int countdown_timer;  // How many turns after applying before countdown_action
	std::string fuel;     // Ammo name - matches this with an ammo type for fuel
	std::string powered_text; // Text for when it's powered; e.g. "on", "active"
};

#endif //CATACLYSM_TYPETOOL_HPP
