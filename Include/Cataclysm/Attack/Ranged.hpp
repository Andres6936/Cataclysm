// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_RANGEDATTACK_HPP
#define CATACLYSM_RANGEDATTACK_HPP

#include <string>

#include <Cataclysm/dice.h>
#include <Cataclysm/enum.h>
#include <Cataclysm/field.h>
#include <Cataclysm/Attack/Enum/RangedHitType.hpp>
#include "Cataclysm/Attack/Enum/RangedAttackType.hpp"

struct Ranged_attack
{
	Ranged_attack();

	~Ranged_attack();

	Ranged_attack_type type;  // Fire, throw, or other (i.e. natural)

	std::string verb_second;
	std::string verb_third;
	int weight;       // Monster attacks - how likely this attack is to be used
	int speed;        // AP used
	int charge_time;  // Monster attacks - how frequently can we use this?
	int range;        // Max range of the attack
	int rounds;       // How many rounds are fired?
	int pellets;      // How many pellets included per round?  Mainly for shot.
	Dice variance;    // In 1/10ths of a degree
	int damage[DAMAGE_MAX];
	int armor_divisor[DAMAGE_MAX];// Times 10; 20 = halve the armor, 5 = double it

/* TODO: Add the following as they're implemented:
 *      Status_effect this causes (Blinding, stunning, etc)
 *      Other???
 */

// Field_pool that we leave in our wake
	Field_pool wake_field;
// Field_pool that is created at the tile hit
	Field_pool target_field;

	bool load_data(std::istream& data, std::string owner_name = "unknown");

	int roll_variance();

	Damage_set roll_damage(Ranged_hit_type hit = RANGED_HIT_NORMAL);
};

#endif //CATACLYSM_RANGEDATTACK_HPP
