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

	public:

		InventoryMultipleSelectionScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_INVENTORYMULTIPLESELECTIONSCREEN_HPP
