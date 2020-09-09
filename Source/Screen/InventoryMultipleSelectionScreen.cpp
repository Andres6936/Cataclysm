// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/InventoryMultipleSelectionScreen.hpp"

using namespace Cataclysm;

InventoryMultipleSelectionScreen::InventoryMultipleSelectionScreen() : IScreen(), InventorySelection()
{
	// Called to parent construct
}

void InventoryMultipleSelectionScreen::draw()
{
	i_inv.draw(console);

	console.draw();
}

void InventoryMultipleSelectionScreen::updated()
{
	if ( not isNeededUpdate) return;

	updateDictionaries();

	i_inv.set_data("text_instructions", "<c=magenta>Press Esc to cancel.\nPress Enter to confirm selection.<c=/>");
	i_inv.set_data("text_after", "<c=brown>After:<c=/>");
	i_inv.set_data("text_after2", "<c=brown>After:<c=/>");

	printDictionaryItems();
	printDictionaryClothing();

	isNeededUpdate = false;
}

ScreenType InventoryMultipleSelectionScreen::processInput()
{
	flushinp();

	const long ch = getch();


}
