// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/String.hpp>
#include <Cataclysm/Screen/Debugger.hpp>
#include <sstream>
#include "Cataclysm/Item/TypeContainer.hpp"


Item_type_container::Item_type_container()
{
	capacity = 0;
	preposition = "of";
	use_article = false;
}

std::string Item_type_container::get_property_description()
{
	std::stringstream ret;
	ret << "<c=white>Capacity:<c=/> " << capacity;
	return ret.str();
}

bool Item_type_container::handle_data(std::string ident, std::istream& data)
{
	std::string junk;
	if (ident == "capacity:")
	{
		data >> capacity;
		std::getline(data, junk);

	}
	else if (ident == "preposition:")
	{
		std::getline(data, preposition);
		preposition = no_caps(preposition);
		preposition = trim(preposition);

	}
	else if (ident == "use_article" || ident == "use_article:")
	{
		use_article = true;

	}
	else if (ident != "done")
	{
		debugmsg("Unknown Container property '%s' (%s)",
				ident.c_str(), name.c_str());
		return false;
	}
	return true;
}

