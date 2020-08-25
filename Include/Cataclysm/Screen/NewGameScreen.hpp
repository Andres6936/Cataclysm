// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_NEWGAMESCREEN_HPP
#define CATACLYSM_NEWGAMESCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class NewGameScreen : public IScreen
	{

		bool userCreatedPlayer = false;

	public:

		NewGameScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_NEWGAMESCREEN_HPP
