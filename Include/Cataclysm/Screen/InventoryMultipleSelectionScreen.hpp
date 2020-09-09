// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INVENTORYMULTIPLESELECTIONSCREEN_HPP
#define CATACLYSM_INVENTORYMULTIPLESELECTIONSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>
#include <Cataclysm/Screen/InventorySelection.hpp>
#include <Cataclysm/Screen/InventoryMultipleSelectionScreen.hpp>

namespace Cataclysm
{

	class InventoryMultipleSelectionScreen : public IScreen, private InventorySelection
	{

	private:

		/**
		 * For avoid duplicate the elements in the inventory of player, is needed
		 * determine when is needed update the screen for show the content of way
		 * consistent, it action too allow a better performance.
		 *
		 * The change of a screen to another too trigger the action of update the
		 * action of update the this screen.
		 */
		bool isNeededUpdate = true;

	public:

		InventoryMultipleSelectionScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_INVENTORYMULTIPLESELECTIONSCREEN_HPP
