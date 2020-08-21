// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include <Cataclysm/stringfunc.h>
#include <Cataclysm/Screen/Debugger.hpp>
#include "Cataclysm/Item/TypeClothing.hpp"


Item_type_clothing::Item_type_clothing()
{
	carry_capacity = 0;
	armor_bash = 0;
	armor_cut = 0;
	armor_pierce = 0;
	protection = 0;
	encumbrance = 0;
	for (int i = 0; i < BODY_PART_MAX; i++)
	{
		covers[i] = false;
	}
}

std::string Item_type_clothing::get_property_description()
{
	std::stringstream ret;
	ret << "<c=white>Covers:<c=/> ";
	bool printed_any = false;
	for (int i = 0; i < BODY_PART_MAX; i++)
	{
		if (covers[i])
		{
			if (printed_any)
			{
				ret << ", ";
			}
			else
			{
				printed_any = true;
			}
			ret << body_part_name(Body_part(i));
		}
	}
	ret << std::endl;

	ret << "<c=ltblue>Storage space: ";
	if (carry_capacity == 0)
	{
		ret << "<c=dkgray>";
	}
	else if (carry_capacity >= 25)
	{  // TODO: Change / don't hardcode this
		ret << "<c=white>";
	}
	else
	{
		ret << "<c=ltgray>";
	}
	ret << carry_capacity << "<c=/>\n";
	ret << "<c=magenta>Encumbrance: ";
	if (encumbrance == 0)
	{
		ret << "<c=white>";
	}
	else if (encumbrance < 3)
	{
		ret << "<c=ltgray>";
	}
	else if (encumbrance < 5)
	{
		ret << "<c=ltred>";
	}
	else
	{
		ret << "<c=red>";
	}
	ret << encumbrance << std::endl;
// Set up vectors for use in color_gradient() below
	std::vector<int> breakpoints;
	breakpoints.push_back(0);
	breakpoints.push_back(10);
	std::vector<nc_color> colors;
	colors.push_back(c_dkgray);
	colors.push_back(c_ltgray);
	colors.push_back(c_white);

	ret << "<c=brown>Armor (bash): " <<
		color_gradient(armor_bash, breakpoints, colors) << armor_bash <<
		"<c=/>\n";
	ret << "<c=brown>Armor (cut): " <<
		color_gradient(armor_cut, breakpoints, colors) << armor_cut <<
		"<c=/>\n";
	ret << "<c=brown>Armor (pierce): " <<
		color_gradient(armor_pierce, breakpoints, colors) << armor_pierce <<
		"<c=/>\n";

	return ret.str();
}


/* TODO:  Right now, armor{_bash,_cut,_pierce} is hard-coded here.  But what if
 *        we add a damage type and want to protect against it with armor?  We
 *        should generalize and look up damage type names instead.
 */
bool Item_type_clothing::handle_data(std::string ident, std::istream& data)
{
	std::string junk;
	if (ident == "carries:")
	{
		data >> carry_capacity;
		std::getline(data, junk);

	}
	else if (ident == "armor_bash:")
	{
		data >> armor_bash;
		std::getline(data, junk);

	}
	else if (ident == "armor_cut:")
	{
		data >> armor_cut;
		std::getline(data, junk);

	}
	else if (ident == "armor_pierce:")
	{
		data >> armor_pierce;
		std::getline(data, junk);

	}
	else if (ident == "protection:")
	{
		data >> protection;
		std::getline(data, junk);

	}
	else if (ident == "encumbrance:")
	{
		data >> encumbrance;
		std::getline(data, junk);

	}
	else if (ident == "covers:")
	{
		std::string line;
		std::getline(data, line);
		std::istringstream cover_data(line);
		std::string body_part_name;
		while (cover_data >> body_part_name)
		{
			std::vector<Body_part> parts = get_body_part_list(body_part_name);
			for (int i = 0; i < parts.size(); i++)
			{
				covers[parts[i]] = true;
			}
			if (parts.empty())
			{
				debugmsg("Unknown body part '%s' (%s)", body_part_name.c_str(),
						name.c_str());
				return false;
			}
		}

	}
	else if (ident != "done")
	{
		return false;
	}
	return true;
}

