// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Mechanism/NextItemManager.hpp"

using namespace Cataclysm;

// Getters

const std::int32_t NextItemManager::getItemUID() noexcept
{
	nextItemUID += 1;

	return nextItemUID;
}

const std::int32_t NextItemManager::getNextItemUid() const noexcept
{
	return nextItemUID;
}
