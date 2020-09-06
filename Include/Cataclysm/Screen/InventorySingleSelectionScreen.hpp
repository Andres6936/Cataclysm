// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP
#define CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>
#include <Cataclysm/Visual/Item/DictionaryItem.hpp>
#include <Cataclysm/Visual/Item/DictionaryClothing.hpp>

namespace Cataclysm
{

	class InventorySingleSelectionScreen : public IScreen
	{

	private:

		cuss::element* ele_list_items;

		cuss::interface i_inv;

		/**
		 * A set of bools, true if the item with that index is selected.
		 */
		std::vector<bool> include_item;

		DictionaryClothing dictionaryClothing;

		/**
		 * Like include_item but for items_worn.
		 */
		std::vector<bool> include_clothing;

		std::vector<char> clothing_letters;

		std::vector<std::string> clothing_name;

		std::vector<std::string> clothing_weight;

		std::vector<std::string> clothing_volume;

		std::vector<int> item_indices[ITEM_CLASS_MAX];

		std::vector<char> item_letters[ITEM_CLASS_MAX];

		std::vector<std::string> item_name;

		std::vector<std::string> item_weight;

		std::vector<std::string> item_volume;

		/**
		 * A bool marked true if we selected our weapon.
		 */
		bool include_weapon = false;

		/**
		 * For avoid duplicate the elements in the inventory of player, is needed
		 * determine when is needed update the screen for show the content of way
		 * consistent, it action too allow a better performance.
		 *
		 * The change of a screen to another too trigger the action of update the
		 * action of update the this screen.
		 */
		bool isNeededUpdate = true;

		int offset = 0;

		int offset_size = 0;

		char weapon_letter = 0;

		void setItemSelected();

		void populate_item_lists(int _offsetSize,
				std::vector<int> _itemIndices[ITEM_CLASS_MAX],
				std::vector<char> _itemLetters[ITEM_CLASS_MAX],
				std::vector<bool> _includeItem,
				std::vector<std::string>& _itemName,
				std::vector<std::string>& _itemWeight,
				std::vector<std::string>& _itemVolume);

	public:

		InventorySingleSelectionScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP
