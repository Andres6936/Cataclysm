// Joan Andrés (@Andres6936) Github.

#include <stdint-gcc.h>
#include "Cataclysm/Item/NextFurnitureManager.hpp"

using namespace Cataclysm;

const std::int32_t NextFurnitureManager::getNextFurniture() noexcept
{
	return nextFurniture += 1;
}
