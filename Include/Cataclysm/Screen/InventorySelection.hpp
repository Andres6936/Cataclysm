// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INVENTORYSELECTION_HPP
#define CATACLYSM_INVENTORYSELECTION_HPP

#include <Cuss/Interface.hpp>
#include <Cataclysm/Item/Enum/Class.hpp>
#include <Cataclysm/Visual/Item/DictionaryAny.hpp>
#include <Cataclysm/Visual/Item/DictionaryItem.hpp>
#include <Cataclysm/Visual/Item/DictionaryClothing.hpp>

namespace Cataclysm
{

	class InventorySelection
	{

	protected:

		// Variables

		inline static cuss::element* ele_list_items {nullptr};

		inline static cuss::interface i_inv;

		inline static DictionaryClothing dictionaryClothing;

		inline static std::array<DictionaryAny, ITEM_CLASS_MAX> dictionaryItems;

		/**
		 * Maximum of elements per each page.
		 */
		inline static int MAXIMUM_ELEMENTS_PER_PAGE = 0;

		/**
		 * A bool marked true if we selected our weapon.
		 */
		inline static bool include_weapon = false;

		inline static char weapon_letter = 0;

		/**
		 * For avoid duplicate the elements in the inventory of player, is needed
		 * determine when is needed update the screen for show the content of way
		 * consistent, it action too allow a better performance.
		 *
		 * The change of a screen to another too trigger the action of update the
		 * action of update the this screen.
		 */
		inline static bool isNeededUpdate = true;

		/**
		 * If have much elements in the inventory of player, is needed use page for
		 * see all the items.
		 */
		inline static int page = 0;

		/**
		 * Is needed have in count the amount of headers for print correctly the items.
		 */
		inline static std::uint32_t totalHeadersInsertedInLastUpdate = 0;

		// Construct

		InventorySelection();

		// Methods

		/**
		 * @post State Inventory have almost 1 item.
		 */
		void setItemSelected();

		void updateDictionaries();

		void markObjectsSelected();

		void printDictionaryItems();

		void printDictionaryClothing();

		/**
		 * @return The total of elements in the dictionary items
		 */
		const std::uint32_t getTotalElementInDictionaryItems() const noexcept;

		const DictionaryItem& getItemAt(const std::uint32_t _index) const;

	};

}

#endif //CATACLYSM_INVENTORYSELECTION_HPP
