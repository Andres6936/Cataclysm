// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_POOL_HPP
#define CATACLYSM_POOL_HPP


#include <map>
#include <memory>
#include <cstdint>

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

class Entity_pool : public std::map<int, std::shared_ptr<Entity>>
{

public:

	void add_entity(std::shared_ptr<Entity> ent); // Assigns a UID

	bool destroyItem(Item* item, std::int32_t _uid = -1);

	bool destroyItemByUID(const std::int32_t _uid);

	Tripoint findItem(Item* item, std::int32_t _uid = -1);

	Tripoint findItemByUID(const std::int32_t _uid);

	std::shared_ptr<Entity> lookup_uid(int uid);

	std::shared_ptr<Entity> entity_at(int posx, int posy);

	std::shared_ptr<Entity> entity_at(Tripoint pos);

	std::shared_ptr<Entity> entity_at(int posx, int posy, int posz);

	std::shared_ptr<Entity> closest_seen_by( int range = -1);

private:

	int next_uid = 0;
};

inline Entity_pool entities {};

#endif //CATACLYSM_POOL_HPP
