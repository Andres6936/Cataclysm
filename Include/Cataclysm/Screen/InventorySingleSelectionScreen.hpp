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

		/**
		 * If have much elements in the inventory of player, is needed use page for
		 * see all the items.
		 */
		int page = 0;

		/**
		 * Maximum of elements per each page.
		 */
		int MAXIMUM_ELEMENTS_PER_PAGE = 0;

		char weapon_letter = 0;

		/**
		 * Is needed have in count the amount of headers for print correctly the items.
		 */
		std::uint32_t totalHeadersInsertedInLastUpdate = 0;

		void setItemSelected();

		void printDictionaryItems();

		void printDictionaryClothing();

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
