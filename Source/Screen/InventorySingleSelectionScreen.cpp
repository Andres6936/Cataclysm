// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Item/TypeClothing.hpp>
#include "Cataclysm/Screen/InventorySingleSelectionScreen.hpp"
#include <Cataclysm/Entity/Player/StateInventory.hpp>

using namespace Cataclysm;

InventorySingleSelectionScreen::InventorySingleSelectionScreen() : IScreen(), InventorySelection()
{
	// Called to parent construct
}

void InventorySingleSelectionScreen::draw()
{
	i_inv.draw(console);

	console.draw();
}

void InventorySingleSelectionScreen::updated()
{
	if ( not isNeededUpdate) return;

	updateDictionaries();

	i_inv.set_data("text_instructions", "<c=magenta>Press Esc to cancel.\nPress - to select nothing.<c=/>");

	printDictionaryItems();
	printDictionaryClothing();

	isNeededUpdate = false;
}

ScreenType InventorySingleSelectionScreen::processInput()
{
	flushinp();

	const long ch = getch();

	if (ch == '-')
	{
		isNeededUpdate = true;

		return ScreenType::PLAY;
	}
	else if (ch == '<' && page > 0)
	{
		page--;

		i_inv.clear_data("list_items");
		i_inv.clear_data("list_weight");
		i_inv.clear_data("list_volume");

		for (int i = page * MAXIMUM_ELEMENTS_PER_PAGE; i < (page + 1) * MAXIMUM_ELEMENTS_PER_PAGE - totalHeadersInsertedInLastUpdate && i < getTotalElementInDictionaryItems(); i++)
		{
			try
			{
				i_inv.add_data("list_items", getItemAt(i).getNameWithLetter());
				i_inv.add_data("list_weight", getItemAt(i).getWeight());
				i_inv.add_data("list_volume", getItemAt(i).getVolume());
			}
			catch (std::out_of_range& exception)
			{
				showDebugMessage(exception.what());
			}
		}
	}
	else if (ch == '>' && getTotalElementInDictionaryItems() > (page + 1) * MAXIMUM_ELEMENTS_PER_PAGE)
	{
		page++;

		i_inv.clear_data("list_items");
		i_inv.clear_data("list_weight");
		i_inv.clear_data("list_volume");

		for (int i = page * MAXIMUM_ELEMENTS_PER_PAGE; i < (page + 1) * MAXIMUM_ELEMENTS_PER_PAGE && i < getTotalElementInDictionaryItems(); i++)
		{
			const std::uint32_t totalHeaders = totalHeadersInsertedInLastUpdate;

			try
			{
				i_inv.add_data("list_items", getItemAt(i - totalHeaders).getNameWithLetter());
				i_inv.add_data("list_weight", getItemAt(i - totalHeaders).getWeight());
				i_inv.add_data("list_volume", getItemAt(i - totalHeaders).getVolume());
			}
			catch (std::out_of_range& exception)
			{
				showDebugMessage(exception.what());
			}
		}
	}
	else if (ch == KEY_ESC)
	{
		isNeededUpdate = true;

		return ScreenType::PLAY;
	}
	else if (ch == '\n')
	{
		try
		{
			setItemSelected();

			return ScreenType::INFORMATION_ITEM;
		}
		catch (std::domain_error& exception)
		{
			showDebugMessage(exception.what());

			return ScreenType::PLAY;
		}
	}
	else
	{ // Anything else warrants a check for the matching key!
		bool found = false;

		if (ch == weapon_letter)
		{
			found = true;
			include_weapon = !include_weapon;
			std::stringstream weapon_ss;
			weapon_ss << (include_weapon ? "<c=green>" : "<c=ltgray>") <<
					  weapon_letter << (include_weapon ? " + " : " - ") <<
					  player->weapon.get_name_full();
			i_inv.set_data("text_weapon", weapon_ss.str());
		}

		// The user has been pressed an key that represent a shortcut to
		// a object in the inventory, in this moment each object have
		// a unique letter or key, is needed found the item that user
		// select
		if (not found)
		{
			for (auto& [clothing, selected] : dictionaryClothing)
			{
				if (clothing.getKey() == ch)
				{
					found = true;

					// Mark the object as selected for the user
					selected = true;
				}
			}
		}

		if (not  found)
		{ // Not the weapon, not clothing - let's check inventory

			for (auto& dictionary : dictionaryItems)
			{
				for (auto& [item, selected] : dictionary)
				{
					if (item.getKey() == ch)
					{
						found = true;

						// Mark the object as selected for the user
						selected = true;
					}
				}
			}
		}

		if (found)
		{
			try
			{
				setItemSelected();

				return ScreenType::INFORMATION_ITEM;
			}
			catch (std::domain_error& exception)
			{
				showDebugMessage(exception.what());

				return ScreenType::PLAY;
			}
		}
	}

	return ScreenType::NONE;
}

void InventorySingleSelectionScreen::setItemSelected()
{
	/**
	 * Important always reset the state of inventory
	 */
	stateInventory.resetState();

	if (include_weapon)
	{
		stateInventory.addItem(player->weapon);
	}

	for (const auto& dictionary : dictionaryItems)
	{
		for (const auto& [item, selected] : dictionary)
		{
			if (selected)
			{
				for (auto& itemInventory: player->inventory)
				{
					if (item.getName() == itemInventory.get_name_full())
					{
						stateInventory.addItem(itemInventory);
					}
				}
			}
		}
	}

	for (const auto& [clothing, selected] : dictionaryClothing)
	{
		if (selected)
		{
			// Search the object in the inventory of player that
			// is represent in the dictionary
			for (auto& item : player->items_worn)
			{
				if (item.get_name_full() == clothing.getName())
				{
					stateInventory.addItem(static_cast<Item_type_clothing*>(item.get_type()));
				}
			}
		}
	}

	// Change of screen with the next action, needed update
	isNeededUpdate = true;

	if (stateInventory.isEmpty())
	{
		throw std::domain_error("State Inventory is empty, post-condition not satisfied");
	}
}

const std::uint32_t InventorySingleSelectionScreen::getTotalElementInDictionaryItems() const noexcept
{
	std::uint32_t totalElements = 0;

	for (const auto& dictionary: dictionaryItems)
	{
		totalElements += dictionary.size();
	}

	// Is important remember that the header too count as a element in the dictionary
	// So that for each dictionary not empty exist a header
	return totalElements + totalHeadersInsertedInLastUpdate;
}

const DictionaryItem& InventorySingleSelectionScreen::getItemAt(const std::uint32_t _index) const
{
	// Verify that _index is in the range of dictionary
	if (_index > getTotalElementInDictionaryItems())
	{
		throw std::out_of_range("Try access to index: " + std::to_string(_index));
	}

	std::uint32_t elementVisited = 0;

	for (const auto& dictionary: dictionaryItems)
	{
		for(const auto& [item, selected] : dictionary)
		{
			if (_index == elementVisited)
			{
				return item;
			}
			else
			{
				elementVisited += 1;
			}
		}
	}

	// This line of code is unreachable
	throw std::out_of_range("Cannot found the element with the index: " + std::to_string(_index));
}
