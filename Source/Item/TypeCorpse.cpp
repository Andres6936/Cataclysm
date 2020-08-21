// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Item/TypeCorpse.hpp"


// Item_type_corpse doesn't have any properties, so this section is boring.
Item_type_corpse::Item_type_corpse()
{
}

bool Item_type_corpse::handle_data(std::string ident, std::istream& data)
{
	if (ident != "done")
	{
		debugmsg("Data in corpse definition - corpse doesn't have data!");
		return false;
	}
	return true;
}

