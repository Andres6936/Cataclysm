#ifndef _ENUM_H_
#define _ENUM_H_

#include <string>
#include <vector> // For get_body_part_list

#include <Cataclysm/Enum/BodyPart.hpp>
#include <Cataclysm/Enum/HealthPointsPart.hpp>

// TODO: Remove these.  For now I'm not sure how to specify them, but no.
#define SIGHT_DIST 20

HP_part lookup_HP_part(std::string name);

std::string HP_part_name(HP_part part);

HP_part convert_to_HP(Body_part part);

#endif
