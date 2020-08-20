// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ACTIVITYTYPE_HPP
#define CATACLYSM_ACTIVITYTYPE_HPP

#include <string>

enum Player_activity_type
{
	PLAYER_ACTIVITY_NULL,
	PLAYER_ACTIVITY_WAIT,
	PLAYER_ACTIVITY_RELOAD,
	PLAYER_ACTIVITY_READ,
	PLAYER_ACTIVITY_MAX
};

std::string get_activity_name(Player_activity_type type);

std::string get_activity_participle(Player_activity_type type);

#endif //CATACLYSM_ACTIVITYTYPE_HPP
