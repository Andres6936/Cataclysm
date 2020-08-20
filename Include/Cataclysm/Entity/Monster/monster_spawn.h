#ifndef _MONSTER_SPAWN_H_
#define _MONSTER_SPAWN_H_

class Monster_genus;
class Monster;

struct Monster_spawn
{
	Monster_spawn();

	~Monster_spawn();

	Monster_genus* genus;
	int population;
// TODO: Other values.  Mobilization?  Status - on alert, passive, aggressive?

	Monster* generate_monster();
};

#endif
