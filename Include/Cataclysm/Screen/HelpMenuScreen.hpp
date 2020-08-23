#ifndef _HELP_H_
#define _HELP_H_

#include <string>
#include "Cataclysm/skill.h"
#include <Cataclysm/Screen/IScreen.hpp>
#include <Cuss/Interface.hpp>

namespace Cataclysm
{
	class HelpMenuScreen : public IScreen
	{

	private:

		Doryen::Console helpConsole {80, 24};

		// This is a list of text files bound to different number input.
		// They'll be looked for in DATA_DIR/help/
		std::vector<std::string> help_files;

		cuss::interface i_help;

	public:

		HelpMenuScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};
}

void help_screen(std::string file = "", std::string term = "");

void help_skill_desc(Skill_type skill);

#endif
