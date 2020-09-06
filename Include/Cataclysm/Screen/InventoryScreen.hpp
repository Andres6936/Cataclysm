// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INVENTORYSCREEN_HPP
#define CATACLYSM_INVENTORYSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class InventoryScreen : public IScreen
	{

	private:

		cuss::element* ele_list_items;

		cuss::interface i_inv;

		std::vector<bool> include_item;

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

		bool single = false;

		bool remove = false;

		// Set static text fields, which are different depending on single/remove
		// So, we have a vector of indices for each item category.
		bool include_weapon = false;

		bool isNeededUpdate = true;

		int offset = 0;

		int offset_size = 0;

		int weight_after = 0;

		int volume_after = 0;

		char weapon_letter = 0;

		void populate_item_lists(int offset_size,
				std::vector<int> item_indices[ITEM_CLASS_MAX],
				std::vector<char> item_letters[ITEM_CLASS_MAX],
				std::vector<bool> include_item,
				std::vector<std::string>& item_name,
				std::vector<std::string>& item_weight,
				std::vector<std::string>& item_volume);

	public:

		InventoryScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_INVENTORYSCREEN_HPP
