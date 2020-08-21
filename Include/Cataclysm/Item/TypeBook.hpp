// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPEBOOK_HPP
#define CATACLYSM_TYPEBOOK_HPP


class Item_type_book : public Item_type
{
public:
	Item_type_book();

	~Item_type_book()
	{
	}

	virtual Item_class get_class()
	{
		return ITEM_CLASS_BOOK;
	}

	virtual Item_action default_action()
	{
		return IACT_READ;
	}

	virtual std::string get_name_singular();  // Slightly more verbose
	virtual std::string get_name_plural();

	virtual std::string get_property_description();

	virtual int time_to_read();

	virtual bool handle_data(std::string ident, std::istream& data);

	Skill_type skill_learned;
	Book_genre genre;
	int cap_limit;  // What's the highest cap this will take us to?
	int int_required;  // What intelligence do we need to read this?
	int skill_required; // What skill level do we need to read this?
	int high_int_bonus; // How much extra do we get for high intelligence?
	int bonus_int_required; // What intelligence do we need to get the bonus?

	int fun;          // Morale gained (or lost) from reading this
	int chapters;     // How many times can we read this before it's "used up?"
	// (Chapters only applies to non-skill books.)
};

#endif //CATACLYSM_TYPEBOOK_HPP
