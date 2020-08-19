#ifndef _KEYBIND_H_
#define _KEYBIND_H_

#include "geometry.h"
#include <map>
#include <string>

enum Interface_action
{
	IACTION_NULL = 0,
	IACTION_SELECT,
// Movement
	IACTION_MOVE_N,
	IACTION_MOVE_NE,
	IACTION_MOVE_E,
	IACTION_MOVE_SE,
	IACTION_MOVE_S,
	IACTION_MOVE_SW,
	IACTION_MOVE_W,
	IACTION_MOVE_NW,
	IACTION_PAUSE,
	IACTION_MOVE_UP,
	IACTION_MOVE_DOWN,
// Map interaction
	IACTION_PICK_UP,
	IACTION_OPEN,
	IACTION_CLOSE,
	IACTION_SMASH,
	IACTION_EXAMINE,
	IACTION_LOOK,
	IACTION_GRAB,
// Inventory interaction
	IACTION_INVENTORY,
	IACTION_DROP,
	IACTION_WIELD,
	IACTION_WEAR,
	IACTION_TAKE_OFF,
	IACTION_APPLY,
	IACTION_READ,
	IACTION_RELOAD_EQUIPPED,
	IACTION_RELOAD,
	IACTION_THROW,
	IACTION_FIRE,
	IACTION_ADVANCE_FIRE_MODE,
	IACTION_EAT,
// Main interface
	IACTION_MESSAGES_SCROLL_BACK,
	IACTION_MESSAGES_SCROLL_FORWARD,
// Other interfaces
	IACTION_VIEW_WORLDMAP,
	IACTION_CHAR_STATUS,
	IACTION_CHAR_SKILLS,
// Other
	IACTION_QUIT,
	IACTION_SAVE,
	IACTION_DEBUG,
	IACTION_MAX
};

Interface_action lookup_interface_action(std::string name);

std::string interface_action_name(Interface_action action);

enum Debug_action
{
	DEBUG_ACTION_NULL = 0,
	DEBUG_ACTION_CREATE_ITEM,
	DEBUG_ACTION_BUILD_MAP,
	DEBUG_ACTION_MAP_INFO,
	DEBUG_ACTION_PLACE_FIELD,
	DEBUG_ACTION_CLEAR_ITEMS,
	DEBUG_ACTION_SPAWN_MONSTER,
	DEBUG_ACTION_MONSTER_PATH,
	DEBUG_ACTION_MEMORY_INFO,
	DEBUG_ACTION_PLACE_BONUS,
	DEBUG_ACTION_GAIN_XP,
	DEBUG_ACTION_MAX
};

Debug_action lookup_debug_action(std::string name);

std::string debug_action_name(Debug_action action);

long lookup_key(std::string name);

struct Keybinding_pool
{
public:
	bool bind_key(long key, Interface_action action);

	Interface_action bound_to_key(long key);

	std::vector<long> keys_bound_to(Interface_action action);

	std::string describe_bindings_for(Interface_action action);

	bool bind_debug_key(long key, Debug_action action);

	Debug_action bound_to_debug_key(long key);

	bool load_from(std::string filename);

private:
	std::map<long, Interface_action> bindings;
	std::map<Interface_action, std::vector<long> > reverse_bindings;

	std::map<long, Debug_action> debug_bindings;
};

Point input_direction(long ch); // Returns "one step in direction bound to ch"

#endif
