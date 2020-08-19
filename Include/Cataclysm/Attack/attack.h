#ifndef _ATTACK_H_
#define _ATTACK_H_

#include <string>

#include <Cataclysm/item.h>
#include <Cataclysm/enum.h>
#include <Cataclysm/skill.h>
#include <Cataclysm/damage_set.h>
#include "Cataclysm/Attack/Enum/MeleHitType.hpp"

Body_part random_body_part_to_hit();

struct Stats;

/* TODO:  Special effects.  It'd be nice to specify that an attack (or an item
 *        from which attacks are derived) has the ability to knock the target
 *        back a tile or two, to poison them, to stun them, etc.
 * TODO:  Special effects bound to damage type.  Like in Cataclysm 1.  Bashing
 *        could stun the target, cutting could cause bleeding or something, and
 *        piercing could "skewer" them, reducing speed but also possibly yanking
 *        the attacker's weapon out of their hands.
 */

struct Attack
{
	std::string verb_second;
	std::string verb_third;
	int weight; // For monster attacks - how likely this attack is to be used
	int speed;
	int to_hit;
	int damage[DAMAGE_MAX];
	bool using_weapon;

	Attack();

	~Attack();

	bool load_data(std::istream& data, std::string owner_name = "unknown");

	void use_weapon(Item weapon, Stats stats);  // Stats is only used for speed
	void adjust_with_stats(Stats stats);        // Adjusts damage
	void adjust_with_skills(Skill_set skills);  // Adjusts damage, speed, to_hit

	Damage_set roll_damage(Melee_hit_type hit_type = MELEE_HIT_NORMAL);

	int roll_damage_type(Damage_type type,
			Melee_hit_type hit_type = MELEE_HIT_NORMAL);
};

#endif
