// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Entity/Player/StateInventory.hpp"

using namespace Cataclysm;

void StateInventory::resetState() noexcept
{
	this->clear();
}

bool StateInventory::isEmpty() const noexcept
{
	return this->empty();
}

void StateInventory::addItem(const Item& item) noexcept
{
	this->push_back(item);
}

const Item& StateInventory::getItemSelected() const noexcept
{
	return (*this)[0];
}

const std::vector<Item>& StateInventory::getItemsSelected() const noexcept
{
	return *this;
}
