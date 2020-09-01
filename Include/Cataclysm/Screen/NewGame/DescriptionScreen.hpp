// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_DESCRIPTIONSCREEN_HPP
#define CATACLYSM_DESCRIPTIONSCREEN_HPP

#include <Cataclysm/Screen/NewGame/Interface/INewGameScreen.hpp>

namespace Cataclysm
{

	class DescriptionScreen : public INewGameScreen
	{

	public:

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_DESCRIPTIONSCREEN_HPP
