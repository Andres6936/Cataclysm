// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_STATSSCREEN_HPP
#define CATACLYSM_STATSSCREEN_HPP

#include <Cataclysm/Screen/NewGame/Interface/INewGameScreen.hpp>

namespace Cataclysm
{

	class StatsScreen : public INewGameScreen
	{

	public:

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_STATSSCREEN_HPP
