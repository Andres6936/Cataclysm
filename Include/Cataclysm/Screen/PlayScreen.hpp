#ifndef _GAME_H_
#define _GAME_H_

#include <Cataclysm/Entity/Pool.hpp>
#include <Cataclysm/Screen/IScreen.hpp>
#include "Cataclysm/Entity/Player/player.h"

class PlayScreen : public Cataclysm::IScreen
{

public:

	PlayScreen();

	// Implemented methods

	void draw() override;

	void updated() override;

	Cataclysm::ScreenType processInput() override;

	// Returns a new world_index

	void reset_temp_values();

	Cataclysm::ScreenType do_action(Interface_action act);

	void move_entities();

	void clean_up_dead_entities();

	void handle_player_activity();

	void complete_player_activity();

	void process_active_items();

/**** Engine - Called-as-needed ****/
	void shift_if_needed();  // Shift the map, if the player's not in the center
	void make_sound(std::string desc, int volume, Tripoint pos);

	void make_sound(std::string desc, int volume, Point pos);

	void make_sound(std::string desc, int volume, int x, int y);

	void make_sound(Sound snd, Tripoint pos);

	void player_move(int xdif, int ydif); // Handles all aspects of moving player
	void player_move_vertical(int zdif);

	// msg_query_yn adds a message, refreshes the HUD, and accepts Y/N input
	bool msg_query_yn(std::string msg, ...);

	void add_active_item(Item* it);

	void remove_active_item(Item* it);

	void remove_active_item_uid(int uid);

	bool destroy_item(Item* it, int uid = -1);

	bool destroy_item_uid(int uid); // destroy_item(NULL, uid)

/**** UI - Output functions ****/
	void draw_all();

	void update_hud();

	void print_messages();

/**** UI - Special screens and inputs ****/
	void debug_command();

	void pickup_items(Tripoint pos);

	void pickup_items(int posx, int posy);

// TODO: Both are limited in that they can not return a point that the player
//       cannot currently see (they return Tripoint() instead).
	Tripoint target_selector(int startx = -1, int starty = -1,
			int range = -1, bool target_entites = false,
			bool show_path = false);

	std::vector<Tripoint> path_selector(int startx = -1, int starty = -1,
			int range = -1,
			bool target_entities = false,
			bool show_path = true);

/**** Data - Universal access functions ****/
	int get_item_uid();     // Get the next available UID (and increment)
	int get_furniture_uid();// Get the next available UID (and increment)
	// Returns true once every $minutes minutes
	// Returns true once every $turns turns
	int get_light_level();          // Current light distance, based on the time

	/* find_item() returns the location of the item.  If it == NULL, use the uid;
 * otherwise, use it.  If it == NULL and uid == -1, just fail immediately.
 * Returns [-1, -1, -1] on fail.
 */
	Tripoint find_item(Item* it, int uid = -1);

	Tripoint find_item_uid(int uid);  // find_item(NULL, uid)

private:

	Doryen::Console screenMap {24, 24};
	Doryen::Console screenHUD {56, 24};

	std::vector<Item*> active_items;


	int next_item_uid;
	int next_furniture_uid;
};

#endif
