// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_CANCELSCREEN_HPP
#define CATACLYSM_CANCELSCREEN_HPP

#include <Cataclysm/Screen/NewGame/Interface/INewGameScreen.hpp>

namespace Cataclysm
{

	class CancelScreen : public INewGameScreen
	{

	public:

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_CANCELSCREEN_HPP
