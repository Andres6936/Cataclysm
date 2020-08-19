// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SPREADTYPE_HPP
#define CATACLYSM_SPREADTYPE_HPP

#include <string>

enum Spread_type
{
	SPREAD_NULL = 0,
	SPREAD_NORMAL,    // Normal spread; slightly weighted towards circular
	SPREAD_CENTER,    // Always spread from the center if we're able
	SPREAD_ARMS,      // Opposite of center - spread while reducing neighbor count
	SPREAD_MAX
};

Spread_type lookup_spread_type(std::string name);

std::string spread_type_name(Spread_type type);

#endif //CATACLYSM_SPREADTYPE_HPP
