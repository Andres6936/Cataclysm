// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Entity/Pool.hpp"
#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Mechanism/NextItemManager.hpp>

Entity_pool::Entity_pool()
{
	next_uid = 0;
}

Entity_pool::~Entity_pool()
{
	for (std::list<Entity*>::iterator it = instances.begin();
		 it != instances.end();
		 it++)
	{
		delete (*it);
	}
}

void Entity_pool::add_entity(Entity* ent)
{
	if (!ent)
	{
		debugmsg("Tried to add_entity NULL to Entity_pool");
		return;
	}
	ent->uid = next_uid;
	next_uid++;
	instances.push_back(ent);
	uid_map[ent->uid] = ent;
}

void Entity_pool::push_back(Entity* ent)
{
	if (!ent)
	{
		debugmsg("Tried to push_back NULL to Entity_pool");
		return;
	}
	instances.push_back(ent);
	uid_map[ent->uid] = ent;
}

void Entity_pool::clear()
{
	instances.clear();
	uid_map.clear();
}

std::list<Entity*>::iterator Entity_pool::erase(std::list<Entity*>::iterator it)
{
	uid_map.erase((*it)->uid);
	return instances.erase(it);
}

bool Entity_pool::empty()
{
	return instances.empty();
}

Entity* Entity_pool::lookup_uid(int uid)
{
	if (uid_map.count(uid) == 0)
	{
		return NULL;
	}
	return uid_map[uid];
}

Entity* Entity_pool::entity_at(int posx, int posy)
{
	for (std::list<Entity*>::iterator it = instances.begin();
		 it != instances.end();
		 it++)
	{
		if ((*it)->pos.x == posx && (*it)->pos.y == posy)
		{
			return (*it);
		}
	}
	return NULL;
}

Entity* Entity_pool::entity_at(Tripoint pos)
{
	return entity_at(pos.x, pos.y, pos.z);
}

Entity* Entity_pool::entity_at(int posx, int posy, int posz)
{
	for (std::list<Entity*>::iterator it = instances.begin();
		 it != instances.end();
		 it++)
	{
		if ((*it)->pos.x == posx && (*it)->pos.y == posy && (*it)->pos.z == posz)
		{
			return (*it);
		}
	}
	return NULL;
}

// range defaults to -1, which means "no range cap"
Entity* Entity_pool::closest_seen_by(Entity* observer, int range)
{
	if (!observer)
	{
		return NULL;
	}
	Tripoint pos = observer->pos;
	int best_range = range;
	Entity* ret = NULL;
	for (std::list<Entity*>::iterator it = instances.begin();
		 it != instances.end();
		 it++)
	{
		Entity* target = *it;
		int dist = rl_dist(pos, target->pos);
		if (target != observer && (best_range == -1 || dist <= best_range) &&
			map->senses(pos, target->pos, range, SENSE_SIGHT))
		{
			best_range = dist;
			ret = target;
		}
	}
	return ret; // Might be NULL!
}

bool Entity_pool::destroyItem(Item* item, std::int32_t _uid)
{
	// Sanity check
	if (item == NULL && (_uid < 0 || _uid >= nextItemManager.getNextItemUid()))
	{
		return false;
	}

	// Check entities first - almost certainly faster than the map
	for (auto& entity : instances)
	{
		Item check = entity->remove_item(item, _uid);

		if (check.is_real())
		{
			return true;
		}
	}

	return map->remove_item(item, _uid);
}

bool Entity_pool::destroyItemByUID(const std::int32_t _uid)
{
	return destroyItem(nullptr, _uid);
}

Tripoint Entity_pool::findItem(Item* item, std::int32_t _uid)
{
	// Sanity check
	if (item == NULL && (_uid < 0 || _uid >= nextItemManager.getNextItemUid()))
	{
		return Tripoint(-1, -1, -1);
	}

	// Check entities first - almost certainly faster than the map
	for (auto& entity : instances)
	{
		if (entity->has_item(item, _uid))
		{
			return entity->pos;
		}
	}

	Tripoint ret = map->find_item(item, _uid);
	return ret;
}

Tripoint Entity_pool::findItemByUID(const std::int32_t _uid)
{
	return findItem(nullptr, _uid);
}


