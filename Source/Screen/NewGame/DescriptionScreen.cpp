// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/files.h>
#include "Cataclysm/Screen/NewGame/DescriptionScreen.hpp"

using namespace Cataclysm;

void DescriptionScreen::draw()
{
	INewGameScreen::draw();
}

void DescriptionScreen::updated()
{
	filenameInterfaceCuss = CUSS_DIR + "/i_newchar_description.cuss";

	INewGameScreen::updated();

	i_newch.ref_data("entry_name", &player->name);

	if (player->male)
	{
		i_newch.set_data("text_male", "<c=yellow>Male<c=/>");
		i_newch.set_data("text_female", "<c=dkgray>Female<c=/>");
	}
	else
	{
		i_newch.set_data("text_male", "<c=dkgray>Male<c=/>");
		i_newch.set_data("text_female", "<c=yellow>Female<c=/>");
	}
}

ScreenType DescriptionScreen::processInput()
{
	flushinp();
	long ch = getch();

	if (ch == '>')
	{
		return ScreenType::NEW_GAME_DONE;
	}
	else if (ch == '<')
	{
		return ScreenType::NEW_GAME_PROFESSION;
	}
	else if (ch == '/')
	{
		player->male = !player->male;
		if (player->male)
		{
			i_newch.set_data("text_male", "<c=yellow>Male<c=/>");
			i_newch.set_data("text_female", "<c=dkgray>Female<c=/>");
		}
		else
		{
			i_newch.set_data("text_male", "<c=dkgray>Male<c=/>");
			i_newch.set_data("text_female", "<c=yellow>Female<c=/>");
		}
	}
	else
	{
		/* Let the interface handle name entry; this includes cursor movement,
		 * backspace, etc.  The only downside is that this allows entry of "invalid"
		 * name characters like "'&^%$#@ etc.  Bad?
		 */
		cuss::element* entry = i_newch.find_by_name("entry_name");
		entry->handle_keypress(ch);
	}

	return ScreenType::NONE;
}
