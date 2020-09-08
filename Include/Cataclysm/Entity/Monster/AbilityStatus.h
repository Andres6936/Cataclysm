#ifndef _MONSTER_ABILITY_H_
#define _MONSTER_ABILITY_H_

#include <Cataclysm/Enum/BodyPart.hpp>
#include "Cataclysm/Entity/Monster/Ability.hpp"

struct Monster_ability_status : public Monster_ability
{
	Monster_ability_status();

	~Monster_ability_status()
	{
	}

	virtual Monster_ability_type type()
	{
		return MON_ABILITY_STATUS;
	}

	virtual bool handle_data(std::string ident, std::istream& data,
			std::string owner);

	virtual bool effect(std::shared_ptr<Monster> user);

	Status_effect_type status;  // Status effect imparted
	int range;  // Range of effect; defaults to 0
	Body_part vector; // Vector of effect; defaults to BODY_PART_NULL (no vector)
	int strength; // How likely it is to be imparted; defaults to 0
	// Set to 0 to guarantee that the effect is imparted.
	Dice duration;  // Duration of effect; defaults to 1
	Dice level;     // Level of effect; defaults to 1
	bool affect_friends;  // If true, affect friends; defaults to false
	bool affect_enemies;  // If true, affect enemies; defaults to false
	bool affect_self;     // If true, affect self;    defaults to false
};

#endif
