// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SHOWWORLDMAPSCREEN_HPP
#define CATACLYSM_SHOWWORLDMAPSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class ShowWorldmapScreen : public IScreen
	{

	private:

		bool updatePoints = true;

		Doryen::Geometry::Point2D<> origin;

		Doryen::Geometry::Point2D<> destination;

		cuss::interface i_legend;

		Doryen::Console legendConsole {20, 24};

		Doryen::Console worldmapConsole {60, 24};

	public:

		ShowWorldmapScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_SHOWWORLDMAPSCREEN_HPP
