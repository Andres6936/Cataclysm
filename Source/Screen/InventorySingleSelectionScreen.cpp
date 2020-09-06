// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/files.h>
#include <Cataclysm/stringfunc.h>
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

	clothing_letters.clear();
	include_clothing.clear();

	clothing_name.clear();
	clothing_volume.clear();
	clothing_weight.clear();

	include_item.clear();

	item_name.clear();
	item_volume.clear();
	item_weight.clear();

	for (int i = 0; i < ITEM_CLASS_MAX; ++i)
	{
		item_indices[i].clear();
	}

	for (int j = 0; j < ITEM_CLASS_MAX; ++j)
	{
		item_letters[j].clear();
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

	for (int i = 0; i < player->items_worn.size(); i++)
	{
		include_clothing.push_back(false);

		std::stringstream clothing_ss;
		Item_type_clothing* clothing =
				static_cast<Item_type_clothing*>(player->items_worn[i].get_type());

		clothing_ss << letter << " - " << player->items_worn[i].get_name_full();
		clothing_name.push_back(clothing_ss.str());

		clothing_weight.push_back(itos(player->items_worn[i].get_weight()));

		int capacity = clothing->carry_capacity;
		if (capacity == 0)
		{
			clothing_volume.push_back("<c=dkgray>+0<c=/>");
		}
		else
		{
			std::stringstream volume_ss;
			volume_ss << "<c=green>+" << capacity << "<c=/>";
			clothing_volume.push_back(volume_ss.str());
		}

		clothing_letters.push_back(letter);
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
		include_item.push_back(false);
		Item_class iclass = player->inventory[i].get_item_class();
		item_indices[iclass].push_back(i);
		item_letters[iclass].push_back(letter);
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

	// Now, populate the string lists
	populate_item_lists(offset_size, item_indices, item_letters,
			include_item, item_name, item_weight, item_volume);

// Set interface data
	i_inv.set_data("weight_current", player->current_weight());
	i_inv.set_data("weight_maximum", player->maximum_weight());
	i_inv.set_data("volume_current", player->current_volume());
	i_inv.set_data("volume_maximum", player->maximum_volume());

	i_inv.set_data("text_instructions", "<c=magenta>Press Esc to cancel.\nPress - to select nothing.<c=/>");

	for (int i = 0; i < offset_size && i < item_name.size(); i++)
	{
		i_inv.add_data("list_items", item_name[i]);
		i_inv.add_data("list_weight", item_weight[i]);
		i_inv.add_data("list_volume", item_volume[i]);
	}
	for (int i = 0; i < offset_size && i < clothing_name.size(); i++)
	{
		i_inv.add_data("list_clothing", clothing_name[i]);
		i_inv.add_data("list_clothing_weight", clothing_weight[i]);
		i_inv.add_data("list_clothing_volume", clothing_volume[i]);
	}

	weight_after = player->current_weight();
	volume_after = player->current_volume();

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
		for (int i = offset * offset_size;
			 i < (offset + 1) * offset_size && i < item_name.size();
			 i++)
		{
			i_inv.add_data("list_items", item_name[i]);
			i_inv.add_data("list_weight", item_weight[i]);
			i_inv.add_data("list_volume", item_volume[i]);
		}
	}
	else if (ch == '>' && item_name.size() > (offset + 1) * offset_size)
	{
		offset++;
		i_inv.clear_data("list_items");
		i_inv.clear_data("list_weight");
		i_inv.clear_data("list_volume");
		for (int i = offset * offset_size;
			 i < (offset + 1) * offset_size && i < item_name.size();
			 i++)
		{
			i_inv.add_data("list_items", item_name[i]);
			i_inv.add_data("list_weight", item_weight[i]);
			i_inv.add_data("list_volume", item_volume[i]);
		}
	}
	else if (ch == KEY_ESC)
	{
		isNeededUpdate = true;

		return ScreenType::PLAY;
	}
	else if (ch == '\n')
	{

		/* If we reach this point, either we're in single-mode and we've selected an
		 * item, or we're in multiple mode and we've hit Enter - either with some items
		 * items selected or without.
		 * Things set at this point:
		 * include_weapon - a bool marked true if we selected our weapon
		 * include_item - a set of bools, true if the item with that index is selected
		 * include_clothing - like include_item but for items_worn
		 */

		stateInventory.resetState();

		if (include_weapon)
		{
			stateInventory.addItem(player->weapon);
		}

		for (int i = 0; i < include_item.size(); i++)
		{
			if (include_item[i])
			{
				stateInventory.addItem(player->inventory[i]);
			}
		}

		for (int i = 0; i < include_clothing.size(); i++)
		{
			if (include_clothing[i])
			{
				stateInventory.addItem(player->items_worn[i]);
			}
		}

		isNeededUpdate = true;

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

		if (!found)
		{
			for (int i = 0; i < clothing_letters.size(); i++)
			{
				if (ch == clothing_letters[i])
				{
					found = true;
					include_clothing[i] = !include_clothing[i];
					bool inc = include_clothing[i];
					std::stringstream clothing_ss;
					clothing_ss << (inc ? "<c=green>" : "<c=ltgray>") <<
								clothing_letters[i] << (inc ? " + " : " - ") <<
								player->items_worn[i].get_name_full();
					clothing_name[i] = clothing_ss.str();
				}
			}
		}

		if (!found)
		{ // Not the weapon, not clothing - let's check inventory
			for (int n = 0; n < ITEM_CLASS_MAX; n++)
			{
				for (int i = 0; i < item_letters[n].size(); i++)
				{
					if (ch == item_letters[n][i])
					{
						found = true;
						int index = item_indices[n][i];
						include_item[index] = !include_item[index];
						bool inc = include_item[index];
						std::stringstream item_ss;
						item_ss << (inc ? "<c=green>" : "<c=ltgray>") <<
								item_letters[n][i] << (inc ? " + " : " - ") <<
								player->inventory[index].get_name_full();
// It's easiest to just set up the text lists for items from scratch!
						populate_item_lists( offset_size, item_indices, item_letters,include_item, item_name, item_weight,item_volume);
					}
				}
			}
		}

		if (found)
		{
			/* If we reach this point, either we're in single-mode and we've selected an
			 * item, or we're in multiple mode and we've hit Enter - either with some items
			 * items selected or without.
			 * Things set at this point:
			 * include_weapon - a bool marked true if we selected our weapon
			 * include_item - a set of bools, true if the item with that index is selected
			 * include_clothing - like include_item but for items_worn
			 */

			stateInventory.resetState();

			if (include_weapon)
			{
				stateInventory.addItem(player->weapon);
			}

			for (int i = 0; i < include_item.size(); i++)
			{
				if (include_item[i])
				{
					stateInventory.addItem(player->inventory[i]);
				}
			}

			for (int i = 0; i < include_clothing.size(); i++)
			{
				if (include_clothing[i])
				{
					stateInventory.addItem(player->items_worn[i]);
				}
			}

			isNeededUpdate = true;

			return ScreenType::PLAY;
		}
	}

	return ScreenType::NONE;
}

void InventorySingleSelectionScreen::populate_item_lists(int _offsetSize, std::vector<int>* _itemIndices,
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
				if (_itemName.size() % _offsetSize == 0)
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
