// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ABILITY_HPP
#define CATACLYSM_ABILITY_HPP


/* Note:
 * All Monster_ability substructs have a function effect(), which the Monster
 * using the ability should call.  This function returns false if the ability
 * was canceled before being attempted!  If the Monster legitimately attempted
 * to use their ability, but failed for some reason, it should return true.
 * The Monster should use this return value to determine whether to use up some
 * AP, etc.
 */

#include <istream>
#include <Cataclysm/Entity/Monster/Enum/AbilityType.hpp>

class Monster;

// Base struct
struct Monster_ability
{
	Monster_ability();

	virtual ~Monster_ability();

	virtual Monster_ability_type type()
	{
		return MON_ABILITY_NULL;
	}

	bool load_data(std::istream& data, std::string owner);

	virtual bool handle_data(std::string ident, std::istream& data,
			std::string owner);

	virtual bool effect(Monster* user);

	int frequency;    // How much using this increases the monster's special_timer
	int weight;       // How likely we are to used this (compared to other abils)
	int AP_cost;      // How much AP using this costs (defaults to 0).
	int HP_cost;      // HP lost when using this ability (defaults to 0).
	std::string verb; // "The <monster_name> <verb>!" (so use 3rd person)

};

#endif //CATACLYSM_ABILITY_HPP
