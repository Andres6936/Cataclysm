#include "Cataclysm/Enum/enum.h"
#include <Cataclysm/Util/String.hpp>

HP_part lookup_HP_part(std::string name)
{
	name = no_caps(name);
	for (int i = 0; i < HP_PART_MAX; i++)
	{
		HP_part ret = HP_part(i);
		if (no_caps(HP_part_name(ret)) == name)
		{
			return ret;
		}
	}
	return HP_PART_NULL;
}

std::string HP_part_name(HP_part part)
{
	switch (part)
	{
	case HP_PART_NULL:
		return "NULL";
	case HP_PART_HEAD:
		return "Head";
	case HP_PART_TORSO:
		return "Torso";
	case HP_PART_LEFT_ARM:
		return "Left Arm";
	case HP_PART_RIGHT_ARM:
		return "Right Arm";
	case HP_PART_LEFT_LEG:
		return "Left Leg";
	case HP_PART_RIGHT_LEG:
		return "Right Leg";
	case HP_PART_MAX:
		return "BUG - HP_PART_MAX";
	default:
		return "BUG - Unnamed HP_part";
	}
	return "BUG - Escaped HP_part_name switch";
}

HP_part convert_to_HP(Body_part part)
{
	switch (part)
	{
	case BODY_PART_EYES:
	case BODY_PART_MOUTH:
	case BODY_PART_HEAD:
		return HP_PART_HEAD;

	case BODY_PART_TORSO:
		return HP_PART_TORSO;

	case BODY_PART_LEFT_HAND:
	case BODY_PART_LEFT_ARM:
		return HP_PART_LEFT_ARM;

	case BODY_PART_RIGHT_HAND:
	case BODY_PART_RIGHT_ARM:
		return HP_PART_RIGHT_ARM;

	case BODY_PART_LEFT_FOOT:
	case BODY_PART_LEFT_LEG:
		return HP_PART_LEFT_LEG;

	case BODY_PART_RIGHT_FOOT:
	case BODY_PART_RIGHT_LEG:
		return HP_PART_RIGHT_LEG;

	default:
		return HP_PART_NULL;
	}
	return HP_PART_NULL;
}


