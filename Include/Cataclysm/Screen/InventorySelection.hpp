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

		inline static char weapon_letter = 0;

		/**
		 * Is needed have in count the amount of headers for print correctly the items.
		 */
		inline static std::uint32_t totalHeadersInsertedInLastUpdate = 0;

		// Construct

		InventorySelection();

		// Methods

		void updateDictionaries();

		void printDictionaryItems();

		void printDictionaryClothing();

	};

}

#endif //CATACLYSM_INVENTORYSELECTION_HPP
