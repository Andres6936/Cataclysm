// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ACTIVEITEMSMANAGER_HPP
#define CATACLYSM_ACTIVEITEMSMANAGER_HPP

#include <vector>
#include <memory>
#include <Cataclysm/Item/item.h>

namespace Cataclysm
{

	class ActiveItemsManager : public std::vector<std::shared_ptr<Item>>
	{

	public:

		void addItem(Item* _item) noexcept;

		void removeItem(Item* _item) noexcept;
	};
}

inline Cataclysm::ActiveItemsManager activeItemsManager {};

#endif //CATACLYSM_ACTIVEITEMSMANAGER_HPP
