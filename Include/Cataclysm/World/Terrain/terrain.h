#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <map>
#include <string>
#include <vector>

#include <Cataclysm/Graphics/glyph.h>
#include "Cataclysm/World/Terrain/Smash.hpp"
#include "Cataclysm/World/Terrain/SignalHandler.hpp"

struct Terrain
{
	int uid;
	std::string name;
	std::string display_name;
	glyph sym;
	unsigned int movecost;
/* Height ranged from 0-100 and reflects how much vertical space is blocked (for
 * line of sight calculations).  It defaults to 100.
 */
	unsigned int height;
	unsigned int hp;      // Defaults to 0.  0 HP means it's indestructible.

	std::string inverse;  // For stairs - the opposite direction

	Terrain_smash smash;
	bool smashable;
	std::string destroy_result;

	// A map of what happens when a tool's signal is applied
	std::map<std::string, Terrain_signal_handler> signal_handlers;

	bool can_smash()
	{
		return smashable;
	}

	Terrain();

	~Terrain()
	{
	}

	std::string get_data_name();

	std::string get_name();

	void assign_uid(int id)
	{
		uid = id;
	}

	bool load_data(std::istream& data);

	bool has_flag(Terrain_flag flag);

private:
	std::vector<bool> flags;
};

#endif
