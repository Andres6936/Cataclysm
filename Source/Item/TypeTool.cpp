// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Item/TypeTool.hpp"


Item_type_tool::Item_type_tool()
{
	def_charges = 0;
	max_charges = 0;
	countdown_timer = 0;
}

// We won't try to handle the Tool_action here - that's better done by hand
std::string Item_type_tool::get_property_description()
{
	std::stringstream ret;
	ret << "<c=yellow>Fuel:<c=/> ";
	if (fuel.empty())
	{
		ret << "<c=dkgray>N/A<c=/>" << std::endl;
	}
	else
	{
		ret << fuel << std::endl;
	}
	ret << "<c=white>Max charges:<c=/> " << max_charges << std::endl;
	ret << "<c=green>Time to apply:<c=/> ";
	if (applied_action.real)
	{
		ret << applied_action.ap_cost / 100 << ".";
		if (applied_action.ap_cost % 100 < 10)
		{
			ret << "0";
		}
		ret << applied_action.ap_cost % 100 << " turns";
	}
	else
	{
		ret << "N/A";
	}
	return ret.str();
}


bool Item_type_tool::handle_data(std::string ident, std::istream& data)
{
	std::string junk;

	if (ident == "applied:")
	{
		if (!applied_action.load_data(data, name))
		{
			return false;
		}

	}
	else if (ident == "powered:")
	{
		if (!powered_action.load_data(data, name))
		{
			return false;
		}

	}
	else if (ident == "countdown:")
	{
		if (!countdown_action.load_data(data, name))
		{
			return false;
		}

	}
	else if (ident == "countdown_timer:")
	{
		data >> countdown_timer;
		std::getline(data, junk);

	}
	else if (ident == "def_charges:" || ident == "default_charges:")
	{
		data >> def_charges;
		std::getline(data, junk);

	}
	else if (ident == "max_charges:")
	{
		data >> max_charges;
		std::getline(data, junk);

	}
	else if (ident == "subcharges:")
	{
		data >> subcharges;
		std::getline(data, junk);

	}
	else if (ident == "fuel:")
	{
		std::getline(data, fuel);
		fuel = no_caps(fuel);
		fuel = trim(fuel);

	}
	else if (ident == "powered_text:")
	{
		std::getline(data, powered_text);
		powered_text = no_caps(powered_text);
		powered_text = trim(powered_text);


	}
	else if (ident != "done")
	{
		debugmsg("Unknown Tool property '%s' (%s)", ident.c_str(), name.c_str());
		return false;
	}
	return true;
}

bool Item_type_tool::uses_charges()
{
	return (max_charges > 0);
}