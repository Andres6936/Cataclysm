// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Cataclysm/Item/TypeBook.hpp"
#include <Cataclysm/Screen/Debugger.hpp>


Item_type_book::Item_type_book()
{
	skill_learned = SKILL_NULL;
	genre = GENRE_NULL;
	cap_limit = 0;
	int_required = 0;
	skill_required = 0;
	high_int_bonus = 0;
	bonus_int_required = 0;
	fun = 0;
	chapters = 1;
}

std::string Item_type_book::get_name_singular()
{
	std::stringstream ret;
	ret << "copy of \"" << get_name() << "\"";
	return ret.str();
}

std::string Item_type_book::get_name_plural()
{
	std::stringstream ret;
	ret << "copies of \"" << get_name() << "\"";
	return ret.str();
}

std::string Item_type_book::get_property_description()
{
	std::stringstream ret;

	if (genre != GENRE_NULL)
	{
		ret << "<c=cyan>Genre: " << book_genre_name(genre) << "<c=/>" <<
			std::endl;
	}

	ret << "<c=magenta>Intelligence required:<c=/> " << int_required << std::endl;

	ret << "<c=brown>Time to read: ";
	if (time_to_read() < 5)
	{
		ret << "<c=white>";
	}
	else if (time_to_read() < 30)
	{
		ret << "<c=yellow>";
	}
	else
	{
		ret << "<c=ltred>";
	}
	ret << time_to_read() << " minutes<c=/>" << std::endl;

	if (skill_learned == SKILL_NULL)
	{
		ret << "<c=dkgray>No related skill.<c=/>" << std::endl;
	}
	else
	{
		ret << "<c=white>Increases your " << skill_type_user_name(skill_learned) <<
			" skill cap to <c=ltblue>" << cap_limit << "<c=/>." << std::endl;
		if (high_int_bonus > 0)
		{
			ret << "<c=white>With intelligence <c=magenta>" << bonus_int_required <<
				"<c=white>, increases your skill cap to <c=ltblue>" <<
				high_int_bonus + cap_limit << "<c=/>." << std::endl;
		}
		if (skill_required > 0)
		{
			ret << "<c=ltred>" << skill_type_user_name(skill_learned) << " level " <<
				"required: " << skill_required;
		}
	}

	if (fun < 0)
	{
		ret << "<c=ltred>Morale lost:<c=ltgray> " << 0 - fun << std::endl;
	}
	else if (fun > 0)
	{
		ret << "<c=ltgreen>Morale gained:<c=ltgray> " << fun << std::endl <<
			"<c=white>Chapters:<c=ltgray> " << chapters << std::endl;
	}

	return ret.str();
}

int Item_type_book::time_to_read()
{
	if (skill_learned == SKILL_NULL)
	{  // It's just a fun book - reads faster
		return 15 + int_required; // More complex books read slower.
	}

	return cap_limit * 5 + int_required * 2;
}

bool Item_type_book::handle_data(std::string ident, std::istream& data)
{
	std::string junk;

	if (ident == "skill:")
	{
		std::string skill_name;
		std::getline(data, skill_name);
		skill_learned = lookup_skill_type(skill_name);
		if (skill_learned == SKILL_NULL)
		{
			debugmsg("Unknown Skill_type '%s' (%s).",
					skill_name.c_str(), name.c_str());
			return false;
		}

	}
	else if (ident == "genre:")
	{
		std::string genre_name;
		std::getline(data, genre_name);
		genre = lookup_book_genre(genre_name);
		if (genre == GENRE_NULL)
		{
			debugmsg("Unknown Book_genre '%s' (%s).",
					genre_name.c_str(), name.c_str());
			return false;
		}

	}
	else if (ident == "cap:")
	{
		data >> cap_limit;
		std::getline(data, junk);

	}
	else if (ident == "int_required:")
	{
		data >> int_required;
		std::getline(data, junk);

	}
	else if (ident == "skill_required:")
	{
		data >> skill_required;
		std::getline(data, junk);

	}
	else if (ident == "high_int_bonus:")
	{
		data >> high_int_bonus;
		std::getline(data, junk);

	}
	else if (ident == "bonus_int_required:")
	{
		data >> bonus_int_required;
		std::getline(data, junk);

	}
	else if (ident == "fun:")
	{
		data >> fun;
		std::getline(data, junk);

	}
	else if (ident == "chapters:")
	{
		data >> chapters;
		std::getline(data, junk);

	}
	else if (ident != "done")
	{
		return false;
	}

	return true;
}

