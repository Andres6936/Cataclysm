// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_PROFESSIONSCREEN_HPP
#define CATACLYSM_PROFESSIONSCREEN_HPP

#include <Cataclysm/Screen/NewGame/Interface/INewGameScreen.hpp>

namespace Cataclysm
{

	class ProfessionScreen : public INewGameScreen
	{

	public:

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_PROFESSIONSCREEN_HPP
