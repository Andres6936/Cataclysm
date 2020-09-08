#include <Cataclysm/Entity/Monster/monster.h>
#include "Cataclysm/Entity/Monster/monster_spawn.h"

Monster_spawn::Monster_spawn()
{
	genus = NULL;
	population = 0;
}

Monster_spawn::~Monster_spawn()
{
}

std::shared_ptr<Monster> Monster_spawn::generate_monster()
{
	if (!genus || population <= 0)
	{
		return NULL;
	}
	std::shared_ptr<Monster> ret = std::make_shared<Monster>();

	ret->set_type(genus->random_member());
	return ret;
}
