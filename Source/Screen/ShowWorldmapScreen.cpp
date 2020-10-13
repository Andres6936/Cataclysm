// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/files.h>
#include "Cataclysm/Screen/ShowWorldmapScreen.hpp"

using namespace Cataclysm;

ShowWorldmapScreen::ShowWorldmapScreen()
{
	i_legend.load_from_file(CUSS_DIR + "/i_world_map.cuss");
}

void ShowWorldmapScreen::draw()
{
	i_legend.draw(legendConsole);

	worldmapConsole.blit({0, 0}, console, {0, 0});
	worldmapConsole.draw();

	legendConsole.blit({0, 0}, console, {60, 0});
	legendConsole.draw();
}

void ShowWorldmapScreen::updated()
{

}

ScreenType ShowWorldmapScreen::processInput()
{
	flushinp();
	long ch = getch();

	if (ch == 'q' or ch == 'Q')
	{
		return ScreenType::PLAY;
	}

	return ScreenType::NONE;
}
