#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include <vector>
#include <istream>

#include "Cataclysm/item_type.h"
#include <Cataclysm/Attack/Ranged.hpp>
#include "Cataclysm/Entity/Monster/monster_type.h"

class Monster_type;

enum Item_active_type
{
	ITEM_ACTIVE_OFF = 0,  // Not active
	ITEM_ACTIVE_POWERED,  // Powered on, like a flashlight
	ITEM_ACTIVE_TIMER,    // Counting down, like a grenade
	ITEM_ACTIVE_MAX
};

class Item
{
public:
	Item(Item_type* T = NULL);

	Item(const Item& rhs);

	~Item();

	Item& operator=(const Item& rhs);

	void set_type(Item_type* T);

	void set_corpse(Monster_type* type)
	{
		corpse = type;
	}

	Item_class get_item_class();

	Item_action default_action();

	bool is_real();

	bool can_reload();

	int time_to_reload();

	int time_to_fire();

	int time_to_read();

// Info fetching
	Item_type* get_type() const;

	int get_uid();

	glyph top_glyph();

// The formal, unique data name
	std::string get_data_name();

// Display name, with corpse descriptor (for corpses)
	std::string get_name();

// These include articles
	std::string get_name_indefinite(); // get_name() + contents
	std::string get_name_definite();   // get_name() only
// No article or count.
	std::string get_name_plural();// Uses type->get_name_plural()
	std::string get_name_full();  // Includes charges, mode, count, powered?, etc

	std::string get_contents_suffix(); // " of soda"; append to other name funcs

	std::string get_description();

	std::string get_description_full();  // Includes type-specific values

	std::vector<Item_action> get_applicable_actions();

	int get_weight();

	int get_volume();

	int get_volume_capacity();

	int get_volume_capacity_used();

	bool has_flag(Item_flag itf);

	bool covers(Body_part part);

	int get_damage(Damage_type dtype);

	int get_to_hit();

	int get_base_attack_speed();

	int get_base_attack_speed(Stats stats);

	int get_shots_fired();

	int get_max_charges();

	bool combines();

	bool combine_by_charges();

	Ranged_attack get_thrown_attack(Entity* ent = NULL);

	Ranged_attack get_fired_attack();

// Changing
	bool combine_with(const Item& rhs);

	//Item in_its_container();
	void prep_for_generation(); // Prepare to be placed by mapgen; e.g. container
	bool place_in_its_container();

	bool add_contents(Item it);

	bool advance_fire_mode(); // For lunachers; change to type's next fire mode
	bool reload(Entity* owner, int ammo_uid);

	bool damage(int dam); // Returns true if the item is destroyed
	bool absorb_damage(Damage_type damtype, int dam); // Returns true if destroyed
/* power_{on,off} toggles ITEM_ACTIVE_POWERED and handles placement in
 * Game::active_items.
 * start_countdown() toggles ITEM_ACTIVE_TIMER and handles placement in
 * Game::active_items.  Not sure if we need/want a stop_countdown().
 */
	bool power_on();

	bool power_off();

	bool start_countdown();

	bool finish_countdown();  // Handles action, removing from active list
	bool is_active(); // True if ITEM_ACTIVE_POWERED or ITEM_ACTIVE_TIMER
// Process_active() returns true if we are not longer active, false otherwise
	bool process_active();

// Interfaces
	Item_action show_info(Entity* user = NULL);

// File I/O
	std::string save_data();

	bool load_data(std::istream& data);

// Data
	Item_type* ammo;  // Currently-loaded ammo type.
	std::vector<Item> contents; // Contents, attached mods, etc.
	int count;
	int charges, subcharges;
	int hp;
	int fire_mode;  // Only applies to launchers (for now?) - it's an index for
	// the type's std::vector<int> modes
private:
	int uid;
	//bool active;
	Item_active_type active;

	Item_type* type;
	Monster_type* corpse;
};

std::string list_items(std::vector<Item>* items);

#endif
