// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/globals.h>
#include <Cataclysm/Screen/MenuScreen.hpp>
#include <Cataclysm/Screen/TourScreen.hpp>
#include <Cataclysm/Screen/SkillScreen.hpp>
#include <Cataclysm/Screen/WorldScreen.hpp>
#include <Cataclysm/Screen/NewGameScreen.hpp>
#include "Cataclysm/Screen/ScreenManager.hpp"
#include <Cataclysm/Screen/HelpMenuScreen.hpp>
#include <Cataclysm/Screen/InventorySingleSelectionScreen.hpp>
#include <Cataclysm/Screen/CreateWorldScreen.hpp>

using namespace Cataclysm;

ScreenManager::ScreenManager()
{
	this->menuScreen = std::make_shared<MenuScreen>();
	this->tourScreen = std::make_shared<TourScreen>();
	this->skillScreen = std::make_shared<SkillScreen>();
	this->worldScreen = std::make_shared<WorldScreen>();
	this->newGameScreen = std::make_shared<NewGameScreen>();
	this->helpMenuScreen = std::make_shared<HelpMenuScreen>();
	this->createWorldScreen = std::make_shared<CreateWorldScreen>();
	this->inventorySingleSelectionScreen = std::make_shared<InventorySingleSelectionScreen>();
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
	else if (_next == ScreenType::PLAY)
	{
		actualScreen = playScreen;
	}
	else if (_next == ScreenType::MENU)
	{
		actualScreen = menuScreen;
	}
	else if (_next == ScreenType::TOUR)
	{
		actualScreen = tourScreen;
	}
	else if (_next == ScreenType::SKILL)
	{
		actualScreen = skillScreen;
	}
	else if (_next == ScreenType::WORLD)
	{
		actualScreen = worldScreen;
	}
	else if (_next == ScreenType::NEW_GAME)
	{
		actualScreen = newGameScreen;
	}
	else if (_next == ScreenType::HELP_MENU)
	{
		actualScreen = helpMenuScreen;
	}
	else if (_next == ScreenType::CREATE_WORLD)
	{
		actualScreen = createWorldScreen;
	}
	else if (_next == ScreenType::INVENTORY_SINGLE_SELECTION)
	{
		actualScreen = inventorySingleSelectionScreen;
	}
}

// Getters

bool ScreenManager::isRunning() const
{
	return running;
}
