// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/NewGameScreen.hpp"

using namespace Cataclysm;

NewGameScreen::NewGameScreen()
{
	currentScreen = statsScreen;
}

void NewGameScreen::draw()
{
	currentScreen->draw();
}

void NewGameScreen::updated()
{
	currentScreen->updated();
}

ScreenType NewGameScreen::processInput()
{
	const ScreenType type = currentScreen->processInput();

	if (type == ScreenType::PLAY)
	{
		return ScreenType::PLAY;
	}
	else if (type == ScreenType::MENU)
	{
		return ScreenType::MENU;
	}
	else if (type == ScreenType::NEW_GAME_DONE)
	{
		currentScreen = doneScreen;
	}
	else if (type == ScreenType::NEW_GAME_STATS)
	{
		currentScreen = statsScreen;
	}
	else if (type == ScreenType::NEW_GAME_CANCEL)
	{
		currentScreen = cancelScreen;
	}
	else if (type == ScreenType::NEW_GAME_TRAITS)
	{
		currentScreen = traitsScreen;
	}
	else if (type == ScreenType::NEW_GAME_PROFESSION)
	{
		currentScreen = professionScreen;
	}
	else if (type == ScreenType::NEW_GAME_DESCRIPTION)
	{
		currentScreen = descriptionScreen;
	}

	return ScreenType::NONE;
}
