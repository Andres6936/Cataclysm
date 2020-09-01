// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/NewGame/CancelScreen.hpp"

using namespace Cataclysm;

void CancelScreen::draw()
{
	INewGameScreen::draw();
}

void CancelScreen::updated()
{
	INewGameScreen::updated();
}

ScreenType CancelScreen::processInput()
{
	if (showQueryYesNo("Cancel character creation?"))
	{
		return ScreenType::MENU;
	}
	else
	{
		cur_screen = New_char_screen::NCS_STATS;
		return ScreenType::NEW_GAME_STATS;
	}
}
