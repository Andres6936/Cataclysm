// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/TourScreen.hpp"
#include <Cataclysm/Util/files.h>

using namespace Cataclysm;

// Construct

TourScreen::TourScreen()
{
	tour_name = CUSS_DIR + "/i_help_interface_" + std::to_string(indexFile) + ".cuss";

	if (!i_tour.load_from_file(tour_name))
	{
		throw std::runtime_error("Cannot found the file " + tour_name );
	}
}

// Methods

void TourScreen::draw()
{
	i_tour.draw(tourScreen);

	tourScreen.blit({0, 0}, console, {0, 0});
	tourScreen.draw();
}

void TourScreen::updated()
{

}

ScreenType TourScreen::processInput()
{
	long ch = getch();

	if (ch == ' ')
	{
		if (indexFile == 6)
		{
			return ScreenType::HELP_MENU;
		}
		else
		{
			// Advance to next section
			indexFile += 1;

			tour_name = CUSS_DIR + "/i_help_interface_" + std::to_string(indexFile) + ".cuss";

			if (!i_tour.load_from_file(tour_name))
			{
				throw std::runtime_error("Cannot found the file " + tour_name );
			}
		}
	}

	return ScreenType::NONE;
}
