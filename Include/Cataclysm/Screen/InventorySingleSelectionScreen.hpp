// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP
#define CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>
#include <Cataclysm/Screen/InventorySelection.hpp>

namespace Cataclysm
{

	class InventorySingleSelectionScreen : public IScreen, private InventorySelection
	{

	public:

		InventorySingleSelectionScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_INVENTORYSINGLESELECTIONSCREEN_HPP
