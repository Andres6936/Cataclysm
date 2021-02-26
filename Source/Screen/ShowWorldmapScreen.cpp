// Joan Andrés (@Andres6936) Github.

#include <Cuss/Graphics/color.h>
#include <Cataclysm/Util/files.h>
#include <Cataclysm/World/map.h>
#include <Cataclysm/World/worldmap.h>
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

	const Point centerPoint = map->get_center_point();
	Point origin = worldmap->get_point(centerPoint.x, centerPoint.y);

	// Adjust to match the upper-left corner
	origin.x -= MAP_SIZE / 2;
	origin.y -= MAP_SIZE / 2;

	for (std::size_t x = 0; x < worldmapConsole.getWidth(); x += 1)
	{
		for(std::size_t y = 0; y < worldmapConsole.getHeight(); y += 1)
		{
			int terx = origin.x + x - (worldmapConsole.getWidth() / 2);
			int tery = origin.y + y - (worldmapConsole.getHeight() / 2);

			glyph symbol = worldmap->get_glyph(terx, tery);

			worldmapConsole.setForegroundColor(Cataclysm::transformColor(symbol.fg));
			worldmapConsole.setBackgroundColor(Cataclysm::transformColor(symbol.bg));
			worldmapConsole.writeChar(x, y, symbol.symbol);
		}
	}

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
