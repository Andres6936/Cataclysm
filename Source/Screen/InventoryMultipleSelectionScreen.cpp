// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/InventoryMultipleSelectionScreen.hpp"

using namespace Cataclysm;

InventoryMultipleSelectionScreen::InventoryMultipleSelectionScreen() : IScreen(), InventorySelection()
{
	// Called to parent construct
}

void InventoryMultipleSelectionScreen::draw()
{
	i_inv.draw(console);

	console.draw();
}

void InventoryMultipleSelectionScreen::updated()
{
	if ( not isNeededUpdate) return;

	updateDictionaries();

	i_inv.set_data("text_instructions", "<c=magenta>Press Esc to cancel.\nPress Enter to confirm selection.<c=/>");
	i_inv.set_data("text_after", "<c=brown>After:<c=/>");
	i_inv.set_data("text_after2", "<c=brown>After:<c=/>");

	const int weight_after = player->current_weight();
	const int volume_after = player->current_volume();

	i_inv.set_data("weight_after", weight_after);
	i_inv.set_data("volume_after", volume_after);

	printDictionaryItems();
	printDictionaryClothing();

	isNeededUpdate = false;
}

ScreenType InventoryMultipleSelectionScreen::processInput()
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
	{
		// Anything else warrants a check for the matching key!
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
			markObjectsSelected();

			i_inv.clear_data("list_clothing");
			i_inv.clear_data("list_clothing_weight");
			i_inv.clear_data("list_clothing_volume");

			printDictionaryClothing();

			i_inv.clear_data("list_items");
			i_inv.clear_data("list_weight");
			i_inv.clear_data("list_volume");

			printDictionaryItems();
		}
	}

	return ScreenType::NONE;
}
