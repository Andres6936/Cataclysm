#ifndef _ITEM_TYPE_H_
#define _ITEM_TYPE_H_


class Item_type
{
public:
	Item_type();

	virtual ~Item_type();

	int uid;
	std::string name;
	std::string display_name;
	std::string plural_name;
	std::string description;
	int weight;       // In 1/10ths of a pound / 0.045 kg (sorry)
	int volume;       // 1 volume = a ping pong ball
	glyph sym;

	int damage[DAMAGE_MAX];
	int to_hit;
	int attack_speed;
	Dice thrown_variance;// Angle our thrown attack is off by, in 1/10ths of
	// a degree; defaults to 5d20!
	int thrown_dmg_percent; // Percent of normal damage done when thrown
	int thrown_speed; // AP cost of throwing; if 0, it's calculated

	int mission_experience; // How much XP does a mission using this item reward
	std::string container;  // Name of another item type!

	void assign_uid(int id);

	std::string get_data_name();

	std::string get_name();

	virtual std::string get_name_singular();  // Slightly more verbose
	virtual std::string get_name_plural();

	virtual bool load_data(std::istream& data);

	virtual bool handle_data(std::string ident, std::istream& data);

	virtual Item_class get_class()
	{
		return ITEM_CLASS_MISC;
	}

	virtual Item_action default_action()
	{
		return IACT_NULL;
	}

	virtual std::string get_property_description()
	{
		return "";
	}

	virtual int time_to_reload()
	{
		return 0;
	}

	virtual int time_to_fire()
	{
		return 0;
	}

	virtual int default_charges()
	{
		return 0;
	}

	virtual bool uses_charges()
	{
		return false;
	}

	virtual bool always_combines()
	{
		return true;
	}

	virtual bool combine_by_charges()
	{
		return false;
	}

	virtual int time_to_read()
	{
		return 0;
	}

	bool has_flag(Item_flag flag);

private:
	std::vector<bool> flags;
};

#endif
