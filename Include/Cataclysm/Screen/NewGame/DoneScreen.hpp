// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_DONESCREEN_HPP
#define CATACLYSM_DONESCREEN_HPP

#include <Cataclysm/Screen/NewGame/Interface/INewGameScreen.hpp>

namespace Cataclysm
{

	class DoneScreen : public INewGameScreen
	{

	public:

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_DONESCREEN_HPP
