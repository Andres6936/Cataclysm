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
	for (std::size_t x = 0; x < worldmapConsole.getWidth(); x += 1)
	{
		for(std::size_t y = 0; y < worldmapConsole.getHeight(); y += 1)
		{
			int terx = destination.x + x - (worldmapConsole.getWidth() / 2);
			int tery = destination.y + y - (worldmapConsole.getHeight() / 2);

			glyph symbol = worldmap->get_glyph(terx, tery);

			worldmapConsole.setForegroundColor(Cataclysm::transformColor(symbol.fg));
			worldmapConsole.setBackgroundColor(Cataclysm::transformColor(symbol.bg));
			worldmapConsole.writeChar(x, y, symbol.symbol);
		}
	}

	i_legend.draw(legendConsole);

	worldmapConsole.blit({0, 0}, console, {0, 0});
	worldmapConsole.draw();

	legendConsole.blit({0, 0}, console, {60, 0});
	legendConsole.draw();
}

void ShowWorldmapScreen::updated()
{
	if (updatePoints)
	{
		const Point centerPoint = map->get_center_point();
		const Point point = worldmap->get_point(centerPoint.x, centerPoint.y);

		// In the first update the origin and destination are equals.
		destination.x = point.x;
		destination.y = point.y;

		// Update the origin point
		origin.x = point.x;
		origin.y = point.y;

		// Adjust to match the upper-left corner
		origin.x -= MAP_SIZE / 2;
		origin.y -= MAP_SIZE / 2;

		// Update the point only once each call to scene
		updatePoints = false;
	}
}

ScreenType ShowWorldmapScreen::processInput()
{
	flushinp();

	long ch = getch();

	if (ch == 'q' or ch == 'Q')
	{
		return ScreenType::PLAY;
	}
	else if (ch == '8')
	{
		destination.y -= 1;
	}
	else if (ch == '4')
	{
		destination.x -= 1;
	}
	else if (ch == '6')
	{
		destination.x += 1;
	}
	else if (ch == '2')
	{
		destination.y += 1;
	}

	return ScreenType::NONE;
}
