// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SIGNALHANDLER_HPP
#define CATACLYSM_SIGNALHANDLER_HPP

#include <list>

#include "Cataclysm/World/Terrain/Bonus.hpp"
#include "Cataclysm/World/Terrain/FlagBonus.hpp"

struct Terrain_signal_handler
{
	Terrain_signal_handler();

	~Terrain_signal_handler()
	{
	}

	bool load_data(std::istream& data, std::string owner_name);

	std::string result;   // The terrain we become
	int success_rate;     // Percentage rate of success

	std::list<Stat_bonus> stat_bonuses;  // Bonuses to success_rate based on stats
	std::list<Terrain_flag_bonus> terrain_flag_bonuses;  // based on terrain_flags

	std::string success_message;    // Message when we succeed
	std::string failure_message;    // Message when we fail
};

#endif //CATACLYSM_SIGNALHANDLER_HPP
