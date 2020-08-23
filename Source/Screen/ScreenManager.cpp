// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/globals.h>
#include <Cataclysm/Screen/MenuScreen.hpp>
#include <Cataclysm/Screen/HelpScreen.hpp>
#include "Cataclysm/Screen/ScreenManager.hpp"

using namespace Cataclysm;

ScreenManager::ScreenManager()
{
	this->menuScreen = std::make_shared<MenuScreen>();
	this->helpMenuScreen = std::make_shared<HelpScreen>();
	this->playScreen.reset(&GAME);

	// The first Screen to show is the Menu
	this->actualScreen = menuScreen;
}

void ScreenManager::draw()
{
	this->actualScreen->draw();
}

void ScreenManager::updated()
{
	this->actualScreen->updated();
}

void ScreenManager::processInput()
{
	nextScene(this->actualScreen->processInput());
}

void ScreenManager::nextScene(ScreenType _next)
{
	if (_next == ScreenType::NONE) return;

	if (_next == ScreenType::QUIT)
	{
		running = false;
	}
	else if (_next == ScreenType::HELP_MENU)
	{
		actualScreen = helpMenuScreen;
	}
}

// Getters

bool ScreenManager::isRunning() const
{
	return running;
}
