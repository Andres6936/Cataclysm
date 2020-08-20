#include "Cataclysm/Entity/Player/player_activity.h"

std::string Player_activity::get_name()
{
	return get_activity_name(type);
}

std::string Player_activity::get_participle()
{
	return get_activity_participle(type);
}
