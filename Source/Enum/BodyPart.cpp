// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Enum/BodyPart.hpp"
#include <Cataclysm/Util/String.hpp> // For no_caps
#include "Cataclysm/Random/rng.h"

Body_part lookup_body_part(std::string name)
{
	name = no_caps(name);
	for (int i = 0; i < BODY_PART_MAX; i++)
	{
		Body_part ret = Body_part(i);
		if (no_caps(body_part_name(ret)) == name)
		{
			return ret;
		}
	}
	return BODY_PART_NULL;
}

std::string body_part_name(Body_part part)
{
	switch (part)
	{
	case BODY_PART_NULL:
		return "NULL";
	case BODY_PART_HEAD:
		return "head";
	case BODY_PART_EYES:
		return "eyes";
	case BODY_PART_MOUTH:
		return "mouth";
	case BODY_PART_TORSO:
		return "torso";
	case BODY_PART_LEFT_HAND:
		return "left hand";
	case BODY_PART_RIGHT_HAND:
		return "right hand";
	case BODY_PART_LEFT_ARM:
		return "left arm";
	case BODY_PART_RIGHT_ARM:
		return "right arm";
	case BODY_PART_LEFT_FOOT:
		return "left foot";
	case BODY_PART_RIGHT_FOOT:
		return "right foot";
	case BODY_PART_LEFT_LEG:
		return "left leg";
	case BODY_PART_RIGHT_LEG:
		return "right leg";
	case BODY_PART_MAX:
		return "BUG - BODY_PART_MAX";
	default:
		return "BUG - Unnamed body part";
	}
	return "BUG - Escaped switch";
}


std::vector<Body_part> get_body_part_list(std::string name)
{
	name = no_caps(name);
	name = trim(name);
	std::vector<Body_part> ret;
	if (name == "hands")
	{
		ret.push_back(BODY_PART_LEFT_HAND);
		ret.push_back(BODY_PART_RIGHT_HAND);
	}
	else if (name == "feet")
	{
		ret.push_back(BODY_PART_LEFT_FOOT);
		ret.push_back(BODY_PART_RIGHT_FOOT);
	}
	else if (name == "arms")
	{
		ret.push_back(BODY_PART_LEFT_ARM);
		ret.push_back(BODY_PART_RIGHT_ARM);
	}
	else if (name == "legs")
	{
		ret.push_back(BODY_PART_LEFT_LEG);
		ret.push_back(BODY_PART_RIGHT_LEG);
	}
	else if (name == "all")
	{
		for (int i = 1; i < BODY_PART_MAX; i++)
		{
			ret.push_back(Body_part(i));
		}
	}
	else
	{
		Body_part bp = lookup_body_part(name);
		if (bp != BODY_PART_NULL)
		{
			ret.push_back(bp);
		}
	}
	return ret;
}

std::string get_protection_meaning(Body_part part)
{
	switch (part)
	{
	case BODY_PART_EYES:
		return "\
Eye protection prevents you from being blinded by chemicals or bright light.";
	case BODY_PART_MOUTH:
		return "\
Mouth protection prevents you from inhaling deadly gases.";
	default:
		return body_part_name(part) + " does not use protection.";
	}
	return "BUG - Escaped switch";
}

Body_part random_head_part()
{
	if (one_in(10))
	{
		return BODY_PART_EYES;
	}
	if (one_in(4))
	{
		return BODY_PART_MOUTH;
	}
	return BODY_PART_HEAD;
}

Body_part random_extremity()
{
	switch (rng(1, 16))
	{
	case 1:
		return BODY_PART_LEFT_HAND;
		break;
	case 2:
		return BODY_PART_RIGHT_HAND;
		break;
	case 3:
		return BODY_PART_LEFT_FOOT;
		break;
	case 4:
		return BODY_PART_RIGHT_FOOT;
		break;
	case 5:
	case 6:
	case 7:
		return BODY_PART_LEFT_ARM;
		break;
	case 8:
	case 9:
	case 10:
		return BODY_PART_RIGHT_ARM;
		break;
	case 11:
	case 12:
	case 13:
		return BODY_PART_LEFT_LEG;
		break;
	case 14:
	case 15:
	case 16:
		return BODY_PART_RIGHT_LEG;
		break;
	}
	return BODY_PART_RIGHT_LEG;
}