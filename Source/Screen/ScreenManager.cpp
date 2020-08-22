// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/globals.h>
#include "Cataclysm/Screen/ScreenManager.hpp"

using namespace Cataclysm;

ScreenManager::ScreenManager()
{
	this->menuScreen.reset(&GAME);
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

	if (_next == ScreenType::QUIT) running = false;
}

// Getters

bool ScreenManager::isRunning() const
{
	return running;
}
