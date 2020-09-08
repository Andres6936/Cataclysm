#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>

#include <Cataclysm/map.h>
#include <Cataclysm/mission.h>
#include <Cataclysm/Attack/attack.h>
#include <Cataclysm/status_effect.h>
#include "Cataclysm/Entity/Stats.hpp"
#include "Cataclysm/Entity/Plan.hpp"
#include <Cataclysm/Enum/BodyPart.hpp>
#include <Cataclysm/Enum/SenseType.hpp>
#include <Cataclysm/Entity/Player/player_activity.h>
#include <Cataclysm/Entity/Player/Enum/ActivityType.hpp>
#include <Cataclysm/tool.h>
#include <Cataclysm/Enum/trait.h>
#include <Cataclysm/Enum/enum.h>

class Entity
{
public:
	Entity();

	virtual ~Entity();

// Name and glyph
	virtual std::string get_data_name();

	virtual std::string get_name();

	virtual std::string get_name_to_player();

	virtual std::string get_possessive();

	virtual std::string conjugate(const std::string& verb);

	virtual glyph get_glyph();

// Entity type
	virtual bool is_player()
	{
		return false;
	}

	virtual bool is_monster()
	{
		return false;
	}

	virtual bool is_you()
	{
		return false;
	} // As in THE player

// Standard turn functions
	virtual void die();

	void start_turn(); // Reset their stats, increase hunger/thirst, etc.

	void adjust_morale();

	void process_status_effects();

	virtual void gain_action_points();

	nc_color get_speed_color();

	virtual int get_speed();

	std::map<std::string, int> get_speed_modifiers();

	Stats get_stats_mod(); // From hunger/thirst/fatigue, status effects, etc
/* The cost of moving onto a 100-cost tile.  For other tiles, the cost is
 * (tile_cost * get_movement_cost()) / 100
 */
	virtual int get_movement_cost();

	int get_hunger_speed_penalty();

	Stats get_hunger_stats_penalty();

	int get_thirst_speed_penalty();

	Stats get_thirst_stats_penalty();

	int get_fatigue_speed_penalty();

	Stats get_fatigue_stats_penalty();

	int get_net_pain();

	int get_pain_speed_penalty();

	int get_smell();

// Experience and morale functions
	void gain_xp(int amount);

	bool improve_skill(Skill_type type);

	void gain_morale(int amount);

	int get_morale();

// Mission functions
/* These are (maybe?) strictly for Player; however, since we actually check
 * mission completion when eating, reading etc., and all those functions are
 * defined for Entity, we need to have access to these functions in Entity as
 * well.
 */

	int personal_mission_cap(); // How many personal missions can we have?
/* Give missions until we have an amount equal to our cap.  By default, add a
 * game message notifying the player; this should be false, for example, when
 * assigning starting missions to a new character.
 */
	void assign_personal_missions(bool message = true);

/* check_mission() will be regularly run in relevent functions, without checking
 * for whether or not we actually HAVE a mission that would be targeted.  For
 * instance. every time we eat something we call
 * check_mission(MISSION_EAT, "food_name").  check_mission() then checks if we
 * have a matching mission, and if so, completes it.
 */
	bool check_mission(Mission_type type, std::string target, int count = 1);

// Flags the mission as complete, gives rewards, and prints messages.
	void complete_mission(int index);

// Removes any completed or failed missions.
	void clean_up_missions();

// Character development functions
	void gain_experience(int amount);

// AI
	virtual void take_turn();

	virtual bool is_enemy(Entity* ent);

	virtual bool try_goal(AI_goal goal);

	virtual bool pick_attack_victim();

	virtual bool pick_flee_target();

	bool has_target();

	bool is_fleeing();

// Type data
	virtual Entity_AI get_AI();

	virtual bool has_sense(Sense_type sense);

	virtual bool has_trait(Trait_id trait);

	virtual int get_genus_uid();

	virtual int get_hunger_minimum();

	virtual int get_thirst_minimum();

	virtual int get_stomach_maximum();

// World interaction
	int sight_range(int light_level = 50);

	virtual bool can_sense(Entity* entity);

	virtual bool can_see(Map* map, Tripoint target);

	virtual bool can_see(Map* map, int x, int y, int z = 999);

	virtual bool can_move_to(Map* map, Tripoint move);

	virtual bool can_move_to(Map* map, int x, int y, int z = 999);

	virtual bool can_drag_furniture_to(Map* map, Tripoint move);

	virtual bool can_drag_furniture_to(Map* map, int x, int y, int z = 999);

	virtual bool can_smash(Map* map, Tripoint move);

	virtual bool can_smash(Map* map, int x, int y, int z = 999);

	virtual void move_to(Map* map, Tripoint move);

	virtual void move_to(Map* map, int x, int y, int z = 999);

	virtual void smash(Map* map, Tripoint sm);

	virtual void smash(Map* map, int x, int y, int z = 999);

	virtual void pause();

	virtual void fall(int levels);  // Doesn't actually move us; handles damage

// Misc action functions
	void set_activity(Player_activity_type type, int duration,
			int primary_uid = -1, int secondary_uid = -1);

	bool has_activity();

	void add_status_effect(Status_effect_type type, int duration, int level = 1);

	void add_status_effect(Status_effect effect);

/* infect() works like add_status_effect(), but it checks our resistance value
 * in the given vector.  For instance, tear gas infects us via the eyes vector
 * (for blinding effect) and the mouth vector (for coughing effect).  Thus, eye
 * protection (e.g. goggles) and mouth protection (e.g. filter) reduce the odds
 * of us suffering the effects.
 */
	void infect(Body_part vector, int strength,
			Status_effect_type type, int duration, int level = 1);

	void infect(Body_part vector, int strength, Status_effect effect);

	bool has_status_effect(Status_effect_type type);

	void use_ap(int amount);

	void shift(int shiftx, int shifty); // For map shifting

// Inventory functions
	virtual bool add_item(Item item);

	Item get_item_of_type(Item_type* type);

	Item* ref_item_of_type(Item_type* type);

	Item* ref_item_uid(int uid);

	Item remove_item(Item* it, int uid, int count = 1);

	Item remove_item_ref(Item* it, int count = 1);

	Item remove_item_uid(int uid, int count = 1);

// These functions just handle the meat - messages are handled in Message below
	void wield_item_uid(int uid);

	void sheath_weapon();

	void wear_item_uid(int uid);

	void take_off_item_uid(int uid);

	void apply_item_uid(int uid);

	void apply_item_action(Item* it, Tool_action* action);

	void read_item_uid(int uid);

	void finish_reading(Item* it);

	virtual bool eat_item_uid(int uid);

	void reload_prep(int uid);

	virtual Item pick_ammo_for(Item* it);

	virtual Tripoint pick_target_for(Item* it);

	int get_chapters_read(std::string title);

	void read_chapter(std::string title);

	bool is_wielding_item_uid(int uid); // is_wielding_item(NULL, uid)
	bool is_wielding_item(Item* it, int uid = -1);

	bool is_wearing_item_uid(int uid); // is_wearing_item(NULL, uid)
	bool is_wearing_item(Item* it, int uid = -1);

	bool is_wearing_item_flag(Item_flag flag);

	bool is_carrying_item_uid(int uid); // is_carrying_item(NULL, uid)
	bool is_carrying_item(Item* it, int uid = -1);

	bool has_item_uid(int uid);         // has_item(NULL, uid)
	bool has_item(Item* it, int uid = -1); // wielding, wearing or carrying

// Message functions
/* TODO (?):  These are all only in entity.cpp, with the assumption that NPCs
 *            will never generate messages like "That is not an article of
 *            clothing" because they'll check beforehand.  Should we instead
 *            move this stuff to player.cpp - if only to make it clear that we
 *            don't expect NPCs to be generating these?
 */
	virtual std::string drop_item_message(Item& it);

	virtual std::string wear_item_message(Item& it);

	virtual std::string take_off_item_message(Item& it);

	virtual std::string wield_item_message(Item& it);

	virtual std::string apply_item_message(Item& it);

	virtual std::string read_item_message(Item& it);

	virtual std::string eat_item_message(Item& it);

	virtual std::string advance_fire_mode_message();  // Only applies to weapon
	virtual std::string sheath_weapon_message();

	std::string get_dragged_name();

// Routine message functions - not tied to any particular action like the above
	std::string get_all_status_text();  // Returns all of the following
	std::string get_hunger_text();

	std::string get_thirst_text();

	std::string get_fatigue_text();

	std::string get_pain_text();

// Combat & HP functions
	virtual Attack base_attack();

	virtual Attack std_attack(); // With weapon if it exists
	virtual bool can_attack(Entity* target);

	virtual void attack(Entity* target);

	virtual int hit_roll(int bonus);

	virtual int dodge_roll();

	virtual void take_damage(Damage_type damtype, int damage, std::string reason,
			Body_part part = BODY_PART_NULL);

	virtual void take_damage_no_armor(Damage_type damtype, int damage,
			std::string reason,
			Body_part part = BODY_PART_NULL);

	virtual void take_damage(Damage_set damage, std::string reason,
			Body_part part = BODY_PART_NULL);

	virtual void take_damage_everywhere(Damage_set damage, std::string reason);

	virtual void take_damage_everywhere(Damage_type type, int damage,
			std::string reason);

	virtual void absorb_damage(Damage_type damtype, int& damage, Body_part part);

	virtual void heal_damage(int damage, HP_part part = HP_PART_NULL);

	virtual int get_armor(Damage_type damtype, Body_part part = BODY_PART_NULL);

// Ambient protection, for use with infect()
	virtual int get_protection(Body_part part = BODY_PART_NULL);

	virtual Ranged_attack throw_item(Item it);

	virtual Ranged_attack fire_weapon();

	virtual std::vector<Ranged_attack> get_ranged_attacks();

	virtual Ranged_attack pick_ranged_attack(Entity* target);

	virtual bool can_fire_weapon();

	virtual bool can_attack_ranged(Entity* target);

	virtual void attack_ranged(Entity* target, Ranged_attack ra);

	virtual bool can_sense(Map* map, int x, int y, int z = 999);

	virtual bool can_sense(Map* map, Tripoint target);

// Values
	int uid;

	Tripoint pos;
	//int posx, posy, posz;
	int action_points;

	bool dead;
	bool killed_by_player;

	Player_activity activity;
	Entity_plan plan;

// Innate attributes
	Stats stats;
	Skill_set skills;
	std::vector<bool> traits;
	int experience;

// Temporary attributes
	int hunger, thirst, fatigue, stomach_food, stomach_water;
	int pain, painkill;
	int special_timer;
	int summons_used;
	int parent_uid;

	std::vector<Mission> missions;

	std::vector<Status_effect> effects;

	Item weapon;
	std::vector<Item> inventory;
	std::vector<Item> items_worn;
// Furniture dragged
	std::vector<Furniture_pos> dragged;
private:
	std::map<std::string, int> chapters_read;
	int morale;
};


#endif
