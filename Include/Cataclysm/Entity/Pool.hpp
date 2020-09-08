// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_POOL_HPP
#define CATACLYSM_POOL_HPP


#include <list>
#include <map>

#include "Cataclysm/Entity/entity.h"

/* For now, Entity_pool does NOT include a map which uses location as a key.
 * Originally I thought this would speed up Game::entity_at(int x, int y), but
 * in order for this map to be useful, we'd have to update it every turn, which
 * means it'd probably be more trouble than it's worth, except when the map is
 * being called several times per turn.  We'd also have to update it after
 * every monster moves, which is a lot.
 * This means that monster_at() has to iterate over all monsters, which is
 * potentially slow, but what can you do.
 */

class Entity_pool : public std::list<Entity*>
{

public:

	Entity_pool();

	virtual ~Entity_pool();

	void add_entity(Entity* ent); // Assigns a UID

	void push_back(Entity* ent); // Same, except don't re-assign UID

	std::list<Entity*>::iterator erase(std::list<Entity*>::iterator it);

	bool destroyItem(Item* item, std::int32_t _uid = -1);

	bool destroyItemByUID(const std::int32_t _uid);

	Tripoint findItem(Item* item, std::int32_t _uid = -1);

	Tripoint findItemByUID(const std::int32_t _uid);

	Entity* lookup_uid(int uid);

	Entity* entity_at(int posx, int posy);

	Entity* entity_at(Tripoint pos);

	Entity* entity_at(int posx, int posy, int posz);

	Entity* closest_seen_by(Entity* observer, int range = -1);

private:

	std::map<int, Entity*> uid_map;

	int next_uid;
};

inline Entity_pool entities {};

#endif //CATACLYSM_POOL_HPP
