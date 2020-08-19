// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SENSETYPE_HPP
#define CATACLYSM_SENSETYPE_HPP

#include <string>

enum Sense_type
{
	SENSE_NULL = 0,     // Can't sense anything
	SENSE_SIGHT,        // Blocked by opaque terrain
	SENSE_SOUND,        // Semi-blocked by solid terrain, only shows noise sources
	SENSE_ECHOLOCATION, // Blocked by solid terrain
	SENSE_SMELL,        // Blocked by solid terrain, only shows scent
	SENSE_INFRARED,     // Not blocked, but only works on warm-blooded targets
	SENSE_OMNISCIENT,   // Not blocked by anything!
	SENSE_MAX
};

Sense_type lookup_sense_type(std::string name);

std::string sense_type_name(Sense_type type);

#endif //CATACLYSM_SENSETYPE_HPP
