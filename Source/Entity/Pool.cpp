// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/World/map.h>
#include "Cataclysm/Entity/Pool.hpp"
#include <Cataclysm/Entity/Player/player.h>
#include <Cataclysm/Mechanism/NextItemManager.hpp>

void Entity_pool::add_entity(std::shared_ptr<Entity> ent)
{
	if (!ent)
	{
		throw std::logic_error("Tried to add_entity NULL to Entity_pool");
	}

	ent->uid = next_uid;
	next_uid++;

	(*this)[ent->uid] = ent;
}

std::shared_ptr<Entity> Entity_pool::lookup_uid(int uid)
{
	if (count(uid) == 0)
	{
		return nullptr;
	}

	return (*this)[uid];
}

std::shared_ptr<Entity> Entity_pool::entity_at(int posx, int posy)
{
	for (auto& [uid, entity] : entities)
	{
		if (entity->pos.x == posx && entity->pos.y == posy)
		{
			return entity;
		}
	}

	return nullptr;
}

std::shared_ptr<Entity> Entity_pool::entity_at(Tripoint pos)
{
	return entity_at(pos.x, pos.y, pos.z);
}

std::shared_ptr<Entity> Entity_pool::entity_at(int posx, int posy, int posz)
{
	for (auto& [uid, entity] : entities)
	{
		if (entity->pos.x == posx && entity->pos.y == posy && entity->pos.z == posz)
		{
			return entity;
		}
	}
	return nullptr;
}

// range defaults to -1, which means "no range cap"
std::shared_ptr<Entity> Entity_pool::closest_seen_by(int range)
{
	Tripoint pos = player->pos;
	int best_range = range;

	std::shared_ptr<Entity> ret { nullptr };

	for (auto& [uid, entity] : entities)
	{
		std::shared_ptr<Entity> target = entity;
		int dist = rl_dist(pos, target->pos);
		if (target.get() != player.get() && (best_range == -1 || dist <= best_range) &&
			::map->senses(pos, target->pos, range, SENSE_SIGHT))
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
	for (auto& [uid, entity] : *this)
	{
		Item check = entity->remove_item(item, _uid);

		if (check.is_real())
		{
			return true;
		}
	}

	return ::map->remove_item(item, _uid);
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
	for (auto& [uid, entity] : *this)
	{
		if (entity->has_item(item, _uid))
		{
			return entity->pos;
		}
	}

	Tripoint ret = ::map->find_item(item, _uid);
	return ret;
}

Tripoint Entity_pool::findItemByUID(const std::int32_t _uid)
{
	return findItem(nullptr, _uid);
}


