// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_WORLDSCREEN_HPP
#define CATACLYSM_WORLDSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class WorldScreen : public IScreen
	{

	private:

		Doryen::Console worldConsole {80, 24};

		cuss::interface i_worlds;

	public:

		// Construct

		WorldScreen();

		// Methods

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_WORLDSCREEN_HPP
