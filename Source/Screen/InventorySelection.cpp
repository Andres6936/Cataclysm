// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/files.h>
#include <Cataclysm/Item/TypeClothing.hpp>
#include <Cataclysm/Entity/Player/player.h>
#include "Cataclysm/Screen/InventorySelection.hpp"
#include <Cataclysm/Entity/Player/StateInventory.hpp>

using namespace Cataclysm;

// Construct

InventorySelection::InventorySelection()
{
	// Lazy evaluation
	if (ele_list_items == nullptr)
	{
		const std::string inv_file = CUSS_DIR + "/i_inventory.cuss";

		// Sanity checks
		if (!i_inv.load_from_file(inv_file))
		{
			throw std::runtime_error("Cannot found the file i_inventory.cuss");
		}

		ele_list_items = i_inv.find_by_name("list_items");

		if (ele_list_items == NULL)
		{
			throw std::logic_error("No element 'list_items' in " + inv_file);
		}

		MAXIMUM_ELEMENTS_PER_PAGE = ele_list_items->sizey;
	}
}

// Methods


void InventorySelection::setItemSelected()
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


void InventorySelection::updateDictionaries()
{
	i_inv.clear_data("list_items");
	i_inv.clear_data("list_weight");
	i_inv.clear_data("list_volume");

	i_inv.clear_data("list_clothing");
	i_inv.clear_data("list_clothing_weight");
	i_inv.clear_data("list_clothing_volume");

	dictionaryClothing.clear();

	for (auto& dictionary : dictionaryItems)
	{
		dictionary.clear();
	}

// Set up letter for weapon, if any exists
	char letter = 'a';

	if (player->weapon.get_type())
	{
		weapon_letter = 'a';
		letter = 'b';
		std::stringstream weapon_ss;
		weapon_ss << weapon_letter << " - " << player->weapon.get_name_full();
		i_inv.set_data("text_weapon", weapon_ss.str());
	}
	else
	{
		i_inv.set_data("text_weapon", "<c=dkgray>No weapon<c=/>");
	}

	// Start with clothing - it's simple!

	for (auto& itemWorn : player->items_worn)
	{
		auto clothing = static_cast<Item_type_clothing*>(itemWorn.get_type());

		const int capacity = clothing->carry_capacity;

		const std::uint16_t key = letter;
		const std::string name = itemWorn.get_name_full();
		const std::string weight = std::to_string(itemWorn.get_weight());
		const std::string volume = capacity == 0 ? "<c=dkgray>+0<c=/>" : "<c=green>+" + std::to_string(capacity) + "<c=/>";

		const DictionaryItem item {key, name, weight, volume};

		dictionaryClothing.emplace(item, false);

		if (letter == 'z')
		{
			letter = 'A';
		}
		else if (letter == 'Z')
		{
			letter = '!';
		}
		else
		{
			letter++;
		}
	}

	// Populate those vectors!

	for (int i = 0; i < player->inventory.size(); i++)
	{
		Item_class iclass = player->inventory[i].get_item_class();

		// Get the dictionary (for reference) that store the
		// representations to object in the inventory of player
		DictionaryAny& dictionary = dictionaryItems.at(iclass);

		const std::uint16_t key = letter;
		const std::string name = player->inventory[i].get_name_full();
		const int weight = player->inventory[i].get_weight();
		const int volume = player->inventory[i].get_volume();

		DictionaryItem item {key, name, weight, volume};

		dictionary.emplace(item, false);

		// TODO: Better inventory letters.  This still isn't unlimited.
		if (letter == 'z')
		{
			letter = 'A';
		}
		else if (letter == 'Z')
		{
			letter = '!';
		}
		else
		{
			letter++;
		}
	}

// Set interface data
	i_inv.set_data("weight_current", player->current_weight());
	i_inv.set_data("weight_maximum", player->maximum_weight());
	i_inv.set_data("volume_current", player->current_volume());
	i_inv.set_data("volume_maximum", player->maximum_volume());
}

void InventorySelection::printDictionaryItems()
{
	// Reset the amount of headers inserted
	totalHeadersInsertedInLastUpdate = 0;

	std::uint32_t counterItemsInserted = 0;

	// Begin from the initial item class enum
	Item_class itemClass = Item_class::ITEM_CLASS_MISC;

	for (const auto& dictionary : dictionaryItems)
	{
		// Only print the name of category if the dictionary not is empty
		if (not dictionary.empty())
		{
			// Create an header with color foreground blue
			i_inv.add_data("list_items", "<c=ltblue>" + item_class_name(itemClass) + "<c=/>");
			i_inv.add_data("list_weight", "");
			i_inv.add_data("list_volume", "");

			totalHeadersInsertedInLastUpdate += 1;
		}

		// Print the list of items
		for (const auto& [item, selected] : dictionary)
		{
			// Not can print more item
			if (counterItemsInserted == MAXIMUM_ELEMENTS_PER_PAGE) break;

			i_inv.add_data("list_items", item.getNameWithLetter());
			i_inv.add_data("list_weight", item.getWeight());
			i_inv.add_data("list_volume", item.getVolume());

			counterItemsInserted += 1;
		}

		// Advance to next class
		itemClass = static_cast<Item_class>(itemClass + 1);
	}
}

void InventorySelection::printDictionaryClothing()
{
	// If the size of elements is lesser than offsetSize thus is sure
	// use a for-loop for print the items
	if (dictionaryClothing.size() < MAXIMUM_ELEMENTS_PER_PAGE)
	{
		for (const auto& [clothing, selected] : dictionaryClothing)
		{
			i_inv.add_data("list_clothing", clothing.getNameWithLetter());
			i_inv.add_data("list_clothing_weight", clothing.getWeight());
			i_inv.add_data("list_clothing_volume", clothing.getVolume());
		}
	}
		// Not is secure use of for-loop, because it can be print elements
		// that not can be see the user in the screen
	else
	{
		int counterElementsInserted = 0;

		for (const auto& [clothing, selected] : dictionaryClothing)
		{
			// If reach the limit of elements that can be visualized in the
			// screen, thus exit
			if (counterElementsInserted == MAXIMUM_ELEMENTS_PER_PAGE) break;

			i_inv.add_data("list_clothing", clothing.getNameWithLetter());
			i_inv.add_data("list_clothing_weight", clothing.getWeight());
			i_inv.add_data("list_clothing_volume", clothing.getVolume());

			counterElementsInserted += 1;
		}
	}
}


const std::uint32_t InventorySelection::getTotalElementInDictionaryItems() const noexcept
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

const DictionaryItem& InventorySelection::getItemAt(const std::uint32_t _index) const
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


using Node = std::multimap<DictionaryItem, bool, DictionaryItemCompare>::node_type;

void markObject(Node& node)
{
	DictionaryItem& item = node.key();

	const std::string nameOriginal = item.getName();

	item.setName("<c=green>" + nameOriginal + "<c=/>");
	item.setKey('-');
}

void InventorySelection::markObjectsSelected()
{
	for (auto& dictionary : dictionaryItems)
	{
		for (auto& [item, selected] : dictionary)
		{
			if (selected)
			{
				Node node = dictionary.extract(item);

				markObject(node);

				dictionary.insert(std::move(node));
			}
		}
	}

	for (auto& [clothing, selected] : dictionaryClothing)
	{
		if (selected)
		{
			Node node = dictionaryClothing.extract(clothing);

			markObject(node);

			dictionaryClothing.insert(std::move(node));
		}
	}
}
