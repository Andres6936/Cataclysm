// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_BODYPART_HPP
#define CATACLYSM_BODYPART_HPP

#include <string>
#include <vector>

enum Body_part
{
	BODY_PART_NULL = 0,
	BODY_PART_HEAD,
	BODY_PART_EYES,
	BODY_PART_MOUTH,
	BODY_PART_TORSO,
	BODY_PART_LEFT_HAND,
	BODY_PART_RIGHT_HAND,
	BODY_PART_LEFT_ARM,
	BODY_PART_RIGHT_ARM,
	BODY_PART_LEFT_FOOT,
	BODY_PART_RIGHT_FOOT,
	BODY_PART_LEFT_LEG,
	BODY_PART_RIGHT_LEG,
	BODY_PART_MAX
};

Body_part lookup_body_part(std::string name);

std::string body_part_name(Body_part part);

// get_body_part_list provides support for "legs" referring to both legs, etc.
std::vector<Body_part> get_body_part_list(std::string name);

// A description of what protection on this part means.
std::string get_protection_meaning(Body_part part);

Body_part random_head_part();

Body_part random_extremity();

#endif //CATACLYSM_BODYPART_HPP
