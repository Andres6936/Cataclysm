// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ABILITYSUMMON_HPP
#define CATACLYSM_ABILITYSUMMON_HPP

#include <Cataclysm/Util/String/var_string.h>
#include <Cataclysm/Random/dice.h>
#include "Cataclysm/Entity/Monster/Ability.hpp"

struct Monster_ability_summon : public Monster_ability
{
	Monster_ability_summon();

	~Monster_ability_summon()
	{
	}

	virtual Monster_ability_type type()
	{
		return MON_ABILITY_SUMMON;
	}

	virtual bool handle_data(std::string ident, std::istream& data,
			std::string owner);

	virtual bool effect(Monster* user);

	Variable_string monster; // So that summons may vary.
	Dice number;  // How many monsters to place.  Defaults to 1.
	int range;  // How far away can the monster be placed?  Defaults to 1.
	int max_summons;  // Maximum # of children we can have. 0 = no limit (default)
};

#endif //CATACLYSM_ABILITYSUMMON_HPP
