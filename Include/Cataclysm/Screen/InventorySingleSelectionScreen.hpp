// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP
#define CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>
#include <Cataclysm/Screen/InventorySelection.hpp>

namespace Cataclysm
{

	class InventorySingleSelectionScreen : public IScreen, private InventorySelection
	{

	private:

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
		 * @post State Inventory have almost 1 item.
		 */
		void setItemSelected();

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
