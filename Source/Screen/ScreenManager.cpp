// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Screen/MenuScreen.hpp>
#include <Cataclysm/Screen/TourScreen.hpp>
#include <Cataclysm/Screen/SkillScreen.hpp>
#include <Cataclysm/Screen/WorldScreen.hpp>
#include <Cataclysm/Screen/NewGameScreen.hpp>
#include "Cataclysm/Screen/ScreenManager.hpp"
#include <Cataclysm/Screen/HelpMenuScreen.hpp>
#include <Cataclysm/Screen/CreateWorldScreen.hpp>
#include <Cataclysm/Screen/InformationItemScreen.hpp>
#include <Cataclysm/Screen/InventorySingleSelectionScreen.hpp>
#include <Cataclysm/Screen/InventoryMultipleSelectionScreen.hpp>

using namespace Cataclysm;

ScreenManager::ScreenManager()
{
	this->menuScreen = std::make_shared<MenuScreen>();
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
		// Lazy evaluation
		if (playScreen == nullptr)
		{
			this->playScreen = std::make_shared<PlayScreen>();
		}

		actualScreen = playScreen;
	}
	else if (_next == ScreenType::MENU)
	{
		actualScreen = menuScreen;
	}
	else if (_next == ScreenType::TOUR)
	{
		// Lazy evaluation
		if (tourScreen == nullptr)
		{
			this->tourScreen = std::make_shared<TourScreen>();
		}

		actualScreen = tourScreen;
	}
	else if (_next == ScreenType::SKILL)
	{
		// Lazy evaluation
		if (skillScreen == nullptr)
		{
			this->skillScreen = std::make_shared<SkillScreen>();
		}

		actualScreen = skillScreen;
	}
	else if (_next == ScreenType::WORLD)
	{
		// Lazy evaluation
		if (worldScreen == nullptr)
		{
			this->worldScreen = std::make_shared<WorldScreen>();
		}

		actualScreen = worldScreen;
	}
	else if (_next == ScreenType::NEW_GAME)
	{
		// Lazy evaluation
		if (newGameScreen == nullptr)
		{
			this->newGameScreen = std::make_shared<NewGameScreen>();
		}

		actualScreen = newGameScreen;
	}
	else if (_next == ScreenType::HELP_MENU)
	{
		// Lazy evaluation
		if (helpMenuScreen == nullptr)
		{
			this->helpMenuScreen = std::make_shared<HelpMenuScreen>();
		}

		actualScreen = helpMenuScreen;
	}
	else if (_next == ScreenType::CREATE_WORLD)
	{
		// Lazy evaluation
		if (createWorldScreen == nullptr)
		{
			this->createWorldScreen = std::make_shared<CreateWorldScreen>();
		}

		actualScreen = createWorldScreen;
	}
	else if (_next == ScreenType::INFORMATION_ITEM)
	{
		// Lazy evaluation
		if (informationItemScreen == nullptr)
		{
			this->informationItemScreen = std::make_shared<InformationItemScreen>();
		}

		actualScreen = informationItemScreen;
	}
	else if (_next == ScreenType::INVENTORY_SINGLE_SELECTION)
	{
		// Lazy evaluation
		if (inventorySingleSelectionScreen == nullptr)
		{
			this->inventorySingleSelectionScreen = std::make_shared<InventorySingleSelectionScreen>();
		}

		actualScreen = inventorySingleSelectionScreen;
	}
	else if (_next == ScreenType::INVENTORY_MULTIPLE_SELECTION)
	{
		// Lazy evaluation
		if (inventoryMultipleSelectionScreen == nullptr)
		{
			this->inventoryMultipleSelectionScreen = std::make_shared<InventoryMultipleSelectionScreen>();
		}

		actualScreen = inventoryMultipleSelectionScreen;
	}
}

// Getters

bool ScreenManager::isRunning() const
{
	return running;
}
