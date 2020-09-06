// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/files.h>
#include <Cataclysm/Item/TypeClothing.hpp>
#include "Cataclysm/Screen/InventorySingleSelectionScreen.hpp"
#include <Cataclysm/Entity/Player/StateInventory.hpp>

using namespace Cataclysm;

InventorySingleSelectionScreen::InventorySingleSelectionScreen()
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

	offset_size = ele_list_items->sizey;
}

void InventorySingleSelectionScreen::draw()
{
	i_inv.draw(console);

	console.draw();
}

void InventorySingleSelectionScreen::updated()
{
	if ( not isNeededUpdate) return;

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

		// Get the dictionary that store the representations to
		// object in the inventory of player
		DictionaryAny dictionary = dictionaryItems.at(iclass);

		const std::uint16_t key = letter;
		const std::string name = player->inventory[i].get_name();
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

	i_inv.set_data("text_instructions", "<c=magenta>Press Esc to cancel.\nPress - to select nothing.<c=/>");

	int counterItemsInserted = 0;

	// Begin from the initial item class enum
	Item_class itemClass = Item_class::ITEM_CLASS_MISC;

	for (const auto& dictionary : dictionaryItems)
	{
		// Only print the name of category if the dictionary not is empty
		if (not dictionary.empty())
		{
			i_inv.add_data("list_items", item_class_name(itemClass));
			i_inv.add_data("list_weight", "");
			i_inv.add_data("list_volume", "");
		}

		// Print the list of items
		for (const auto& [item, selected] : dictionary)
		{
			i_inv.add_data("list_items", item.getNameWithLetter());
			i_inv.add_data("list_weight", item.getWeight());
			i_inv.add_data("list_volume", item.getVolume());

			counterItemsInserted += 1;

			// Not can print more item
			if (counterItemsInserted == offset_size) break;
		}

		// Advance to next class
		itemClass = static_cast<Item_class>(itemClass + 1);

		// Not can print more item
		if (counterItemsInserted == offset_size) break;
	}

	// If the size of elements is lesser than offsetSize thus is sure
	// use a for-loop for print the items
	if (dictionaryClothing.size() < offset_size)
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
			i_inv.add_data("list_clothing", clothing.getNameWithLetter());
			i_inv.add_data("list_clothing_weight", clothing.getWeight());
			i_inv.add_data("list_clothing_volume", clothing.getVolume());

			// If reach the limit of elements that can be visualized in the
			// screen, thus exit
			if (counterElementsInserted == offset_size)
			{
				break;
			}
			else
			{
				counterElementsInserted += 1;
			}
		}
	}

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
	else if (ch == '<' && offset > 0)
	{
		offset--;
		i_inv.clear_data("list_items");
		i_inv.clear_data("list_weight");
		i_inv.clear_data("list_volume");
//		for (int i = offset * offset_size;
//			 i < (offset + 1) * offset_size && i < item_name.size();
//			 i++)
//		{
//			i_inv.add_data("list_items", item_name[i]);
//			i_inv.add_data("list_weight", item_weight[i]);
//			i_inv.add_data("list_volume", item_volume[i]);
//		}
	}
//	else if (ch == '>' && item_name.size() > (offset + 1) * offset_size)
//	{
//		offset++;
//		i_inv.clear_data("list_items");
//		i_inv.clear_data("list_weight");
//		i_inv.clear_data("list_volume");
//		for (int i = offset * offset_size;
//			 i < (offset + 1) * offset_size && i < item_name.size();
//			 i++)
//		{
//			i_inv.add_data("list_items", item_name[i]);
//			i_inv.add_data("list_weight", item_weight[i]);
//			i_inv.add_data("list_volume", item_volume[i]);
//		}
//	}
	else if (ch == KEY_ESC)
	{
		isNeededUpdate = true;

		return ScreenType::PLAY;
	}
	else if (ch == '\n')
	{
		setItemSelected();

		return ScreenType::PLAY;
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
			setItemSelected();

			return ScreenType::PLAY;
		}
	}

	return ScreenType::NONE;
}

void InventorySingleSelectionScreen::populate_item_lists( std::vector<int>* _itemIndices,
		std::vector<char>* _itemLetters, std::vector<bool> _includeItem, std::vector<std::string>& _itemName,
		std::vector<std::string>& _itemWeight, std::vector<std::string>& _itemVolume)
{
	_itemName.clear();
	_itemWeight.clear();
	_itemVolume.clear();

	for (int n = 0; n < ITEM_CLASS_MAX; n++)
	{
		if (!_itemIndices[n].empty())
		{
			const std::string class_name = "<c=ltblue>" + item_class_name_plural(Item_class(n)) + "<c=/>";

			// Create a header
			_itemName.push_back(class_name);
			_itemWeight.push_back("");
			_itemVolume.push_back("");

			for (int i = 0; i < _itemIndices[n].size(); i++)
			{

				// Check to see if we're starting a new page.
				// If so, repeat the category header
				if (_itemName.size() % offset_size == 0)
				{
					_itemName.push_back(class_name + "(cont)");
					_itemWeight.push_back("");
					_itemVolume.push_back("");
				}

				const int index = _itemIndices[n][i];

				Item& item = player->inventory[index];

				const bool inc = _includeItem[index];

				std::stringstream item_ss, weight_ss, volume_ss;

				item_ss << (inc ? "<c=green>" : "<c=ltgray>") << _itemLetters[n][i] <<
						(inc ? " + " : " - ") << item.get_name_full() << "<c=/>";
				_itemName.push_back(item_ss.str());

				weight_ss << (inc ? "<c=green>" : "<c=ltgray>") << item.get_weight() << "<c=/>";
				_itemWeight.push_back(weight_ss.str());

				volume_ss << (inc ? "<c=green>" : "<c=ltgray>") << item.get_volume() << "<c=/>";
				_itemVolume.push_back(volume_ss.str());
			}
		}
	}
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
}
