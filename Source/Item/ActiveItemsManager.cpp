// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Item/ActiveItemsManager.hpp"

using namespace Cataclysm;

void ActiveItemsManager::addItem(Item* _item) noexcept
{
	if (!_item)
	{
		return;
	}

	std::shared_ptr<Item> item {_item};

	push_back(item);
}

void ActiveItemsManager::removeItem(Item* _item) noexcept
{
	if (!_item)
	{
		return;
	}

	for (int i = 0; i < size(); i++)
	{
		if ((*this)[i].get() == _item)
		{
			erase(begin() + i);
			return;
		}
	}
}
