#ifndef _MONSTER_TYPE_H_
#define _MONSTER_TYPE_H_

#include <string>

enum Monster_size
{
	MON_SIZE_NULL = 0,
	MON_SIZE_TINY,      // Up to cat size
	MON_SIZE_SMALL,     // Up to wolf size
	MON_SIZE_MEDIUM,    // Human-size
	MON_SIZE_LARGE,     // Cow-size
	MON_SIZE_HUGE,      // Elephant-size
	MON_SIZE_MAX
};

Monster_size lookup_monster_size(std::string name);

std::string monster_size_name(Monster_size size);

#endif
