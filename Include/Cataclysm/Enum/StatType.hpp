// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_STATTYPE_HPP
#define CATACLYSM_STATTYPE_HPP

#include <string>

enum Stat_id
{
	STAT_NULL = 0,
	STAT_STRENGTH,
	STAT_DEXTERITY,
	STAT_INTELLIGENCE,
	STAT_PERCEPTION,
	STAT_MAX
};

Stat_id lookup_stat_id(std::string name);

std::string stat_id_name(Stat_id id);

std::string stat_id_short_name(Stat_id id);

#endif //CATACLYSM_STATTYPE_HPP
