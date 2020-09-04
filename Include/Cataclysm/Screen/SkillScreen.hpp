// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SKILLSCREEN_HPP
#define CATACLYSM_SKILLSCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class SkillScreen : public IScreen
	{

	private:

		Doryen::Console skillScreen {80, 24};

		cuss::interface i_skills;

		bool helpMode = false;

	public:

		SkillScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_SKILLSCREEN_HPP
