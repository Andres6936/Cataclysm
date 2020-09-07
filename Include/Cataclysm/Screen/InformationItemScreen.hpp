// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INFORMATIONITEMSCREEN_HPP
#define CATACLYSM_INFORMATIONITEMSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class InformationItemScreen : public IScreen
	{

	private:

		cuss::interface i_info;

		std::vector<Item_action> app_actions;

	public:

		InformationItemScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_INFORMATIONITEMSCREEN_HPP
