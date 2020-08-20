// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ABILITYSIGNAL_HPP
#define CATACLYSM_ABILITYSIGNAL_HPP

#include <Cataclysm/var_string.h>
#include "Cataclysm/Entity/Monster/Ability.hpp"

// Right now, Monster_ability_signal applies to ALL terrain within <range>.
// We can change this if so desired, but choosing a target may be VERY complex.
struct Monster_ability_signal : public Monster_ability
{
	Monster_ability_signal();

	~Monster_ability_signal()
	{
	}

	virtual Monster_ability_type type()
	{
		return MON_ABILITY_SIGNAL;
	}

	virtual bool handle_data(std::string ident, std::istream& data,
			std::string owner);

	virtual bool effect(Monster* user);

	Variable_string signal; // A signal to be sent to terrain (may be randomized)
	int range;  // Radius of ability; defaults to 1
};

#endif //CATACLYSM_ABILITYSIGNAL_HPP
