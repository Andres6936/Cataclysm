// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/NewGame/DoneScreen.hpp"

using namespace Cataclysm;

void DoneScreen::draw()
{
	INewGameScreen::draw();
}

void DoneScreen::updated()
{
	INewGameScreen::updated();
}

ScreenType DoneScreen::processInput()
{
	std::string reason_for_fail;

	if (points > 0)
	{
		reason_for_fail += "\nYou have unspent points!";
	}
	if (player->profession == nullptr)
	{
		reason_for_fail += "\nYou didn't choose a profession!";
	}
	if (player->name.empty())
	{
		reason_for_fail += "\nYour name is blank!";
	}

	if (!reason_for_fail.empty())
	{
		showDebugMessage(Doryen::format("Wait, you can't start the game yet! {}", reason_for_fail));
	}
	else if (showQueryYesNo("Complete character and start the game?"))
	{
		verifyInvariantPlayer();

		// Exit of Scene if the user has been created successfully a player
		return ScreenType::PLAY;
	}

	cur_screen = New_char_screen::NCS_DESCRIPTION;

	return ScreenType::NEW_GAME_DESCRIPTION;
}
