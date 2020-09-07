// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP
#define CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>
#include <Cataclysm/Visual/Item/DictionaryAny.hpp>
#include <Cataclysm/Visual/Item/DictionaryItem.hpp>
#include <Cataclysm/Visual/Item/DictionaryClothing.hpp>

namespace Cataclysm
{

	class InventorySingleSelectionScreen : public IScreen
	{

	private:

		cuss::element* ele_list_items;

		cuss::interface i_inv;

		DictionaryClothing dictionaryClothing;

		std::array<DictionaryAny, ITEM_CLASS_MAX> dictionaryItems;

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

		void printDictionaryItems();

		void printDictionaryClothing();

		const std::uint32_t getAmountOfHeaders() const noexcept;

		/**
		 * @return The total of elements in the dictionary items
		 */
		const std::uint32_t getTotalElementInDictionaryItems() const noexcept;

		const DictionaryItem& getItemAt(const std::uint32_t _index) const;

	public:

		InventorySingleSelectionScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP
