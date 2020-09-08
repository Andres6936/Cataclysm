// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_MENUSCREEN_HPP
#define CATACLYSM_MENUSCREEN_HPP

#include <Cuss/Interface.hpp>
#include <Cataclysm/window.h>
#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class MenuScreen : public IScreen
	{

	private:

		cuss::interface i_menu;

	public:

		// Construct

		MenuScreen();

		// Methods

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_MENUSCREEN_HPP
