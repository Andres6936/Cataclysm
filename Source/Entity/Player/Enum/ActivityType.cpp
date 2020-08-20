// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Entity/Player/Enum/ActivityType.hpp"


std::string get_activity_name(Player_activity_type type)
{
	switch (type)
	{
	case PLAYER_ACTIVITY_NULL:
		return "nothing";
	case PLAYER_ACTIVITY_WAIT:
		return "wait";
	case PLAYER_ACTIVITY_RELOAD:
		return "reload";
	case PLAYER_ACTIVITY_READ:
		return "read";
	case PLAYER_ACTIVITY_MAX:
		return "BUG - PLAYER_ACTIVITY_MAX";

	default:
		return "BUG - Unnamed Player_activity_type";
	}
	return "BUG - Escaped switch (get_activity_name())";
}

std::string get_activity_participle(Player_activity_type type)
{
	switch (type)
	{
	case PLAYER_ACTIVITY_NULL:
		return "doing nothing";
	case PLAYER_ACTIVITY_WAIT:
		return "waiting";
	case PLAYER_ACTIVITY_RELOAD:
		return "reloading";
	case PLAYER_ACTIVITY_READ:
		return "reading";
	case PLAYER_ACTIVITY_MAX:
		return "BUG - PLAYER_ACTIVITY_MAX";

	default:
		return "BUG - Unparticiple Player_activity_type";
	}
	return "BUG - Escaped switch (get_activity_participle())";
}


