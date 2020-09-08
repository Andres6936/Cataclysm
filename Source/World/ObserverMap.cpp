// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/World/map.h>
#include <Cataclysm/Entity/Pool.hpp>
#include "Cataclysm/World/ObserverMap.hpp"

using namespace Cataclysm;

bool ObserverMap::isEmpty(const Tripoint& _position) const noexcept
{
	return (!(entities.entity_at(_position)) && map->move_cost(_position) > 0);
}
