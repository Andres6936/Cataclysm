// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ABILITYTELEPORT_HPP
#define CATACLYSM_ABILITYTELEPORT_HPP

#include "Cataclysm/Entity/Monster/Ability.hpp"

struct Monster_ability_teleport : public Monster_ability
{
	Monster_ability_teleport();

	~Monster_ability_teleport()
	{
	}

	virtual Monster_ability_type type()
	{
		return MON_ABILITY_TELEPORT;
	}

	virtual bool handle_data(std::string ident, std::istream& data,
			std::string owner);

	virtual bool effect(Monster* user);

	int range;  // Max range of teleport.
/* If <always_use_max_range> is true, we'll use a random open tile exactly
 * <range> tiles away.  If none are available, we'll try <range> - 1, and so on.
 * If false, we'll teleport to any open tile within <range>.
 * Defaults to false.
 */
	bool always_use_max_range;
/* If <controlled> is false, then the teleport is truly random; any available
 * tile is a potential result.
 * If false, then we'll teleport as close as possible to our target if we're
 * attacking; or as far as possible from our target if we're fleeing.
 * Defaults to false.
 */
	bool controlled;
/* If <phase> is true, then any tile within <range> is a valid target.
 * If false, then only targets which we can sense AND which have a uninterrupted
 * line to them are valid.
 * Defaults to false.
 */
	bool phase;
};

#endif //CATACLYSM_ABILITYTELEPORT_HPP
