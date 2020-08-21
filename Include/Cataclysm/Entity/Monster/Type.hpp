// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TYPE_HPP
#define CATACLYSM_TYPE_HPP


#include <string>

#include <Cataclysm/dice.h>
#include <Cataclysm/sound.h>
#include <Cataclysm/glyph.h>
#include <Cataclysm/entity_ai.h>
#include <Cataclysm/var_string.h>
#include <Cataclysm/Attack/attack.h>
#include <Cataclysm/Attack/Ranged.hpp>
#include <Cataclysm/Enum/SenseType.hpp>
#include <Cataclysm/Entity/Monster/Ability.hpp>
#include "Size.h"

class Monster_genus;

struct Monster_type
{
	Monster_type();

	~Monster_type();

	Monster_genus* genus; // See below
	std::string name;     // UNIQUE Data name
	std::string display_name; // Name as the player sees it; if blank, use (name)
	std::string display_name_plural;  // Plural name
	int uid;  // Unique UID for this type
	glyph sym;  // See glyph.h

	Monster_size size;
	Dice hp_dice; // Dice to roll to determine HP; may be static!
	std::vector<int> armor;
	int speed;    // 100 = player's base speed
	int accuracy; // Our melee accuracy; compared against target's dodge
	int dodge;    // Rolled against attacker's hit roll
	int chance;   // How frequently this appears

	std::vector<Attack> attacks;  // Melee attacks - see attack.h
	int total_attack_weight;  // Variable for choosing an attack

	std::vector<Ranged_attack> ranged_attacks;  // See attack.h
	int total_ranged_attack_weight; // Variable for choosing a ranged attack

	std::vector<Monster_ability*> abilities; // See monster_ability.h
	int total_ability_weight;

	Entity_AI AI; // AI set

	void set_genus(Monster_genus* mg);

	void assign_uid(int id);

	std::string get_data_name();

	std::string get_name();

	std::string get_name_plural();

	bool load_data(std::istream& data);

	bool has_sense(Sense_type type);

	int get_weight();

	int get_volume();

	Sound get_sound(bool attacking);

	Monster_ability* get_ability();

private:
// These bools determine whether we've copied data from our genus
	bool attacks_copied_from_genus;
	bool ranged_attacks_copied_from_genus;
	bool abilities_copied_from_genus;
	bool senses_copied_from_genus;
	bool armor_copied_from_genus;
	bool hp_set;  // Temp variable used to copy from genus
	std::vector<bool> senses;
	Variable_string idle_sounds;
	Variable_string attack_sounds;
	int idle_sound_chance;
	int attack_sound_chance;
	int idle_sound_volume;
	int attack_sound_volume;

};

#endif //CATACLYSM_TYPE_HPP
