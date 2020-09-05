// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_STATEINVENTORY_HPP
#define CATACLYSM_STATEINVENTORY_HPP

#include <vector>
#include <Cataclysm/Item/item.h>

namespace Cataclysm
{

	class StateInventory : private std::vector<Item>
	{

	public:

		void resetState() noexcept;

		bool isEmpty() const noexcept;

		void addItem(const Item& item) noexcept;

		const Item& getItemSelected() const noexcept;

		const std::vector<Item>& getItemsSelected() const noexcept;

	};

}

inline Cataclysm::StateInventory stateInventory {};

#endif //CATACLYSM_STATEINVENTORY_HPP
