// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SHOWWORLDMAPSCREEN_HPP
#define CATACLYSM_SHOWWORLDMAPSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class ShowWorldmapScreen : public IScreen
	{

	public:

		ShowWorldmapScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_SHOWWORLDMAPSCREEN_HPP
