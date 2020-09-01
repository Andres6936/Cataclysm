// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TRAITSSCREEN_HPP
#define CATACLYSM_TRAITSSCREEN_HPP

#include <Cataclysm/Screen/NewGame/Interface/INewGameScreen.hpp>

namespace Cataclysm
{

	class TraitsScreen : public INewGameScreen
	{

	public:

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_TRAITSSCREEN_HPP
