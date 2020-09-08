// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/WorldScreen.hpp"
#include <Cataclysm/Util/String/stringfunc.h>
#include <Cataclysm/Util/files.h>

using namespace Cataclysm;

// Construct

WorldScreen::WorldScreen()
{
	if (!i_worlds.load_from_file(CUSS_DIR + "/i_worlds.cuss"))
	{
		throw std::runtime_error("Cannot found i_worlds.cuss");
	}

	i_worlds.set_data("list_worlds", worldmap_names);
	i_worlds.select("list_worlds");
}

// Methods

void WorldScreen::draw()
{
	i_worlds.draw(worldConsole);

	worldConsole.blit({0, 0}, console, {0, 0});
	worldConsole.draw();
}

void WorldScreen::updated()
{
	// Repopulate list_worlds with (hopefully) a new world name.
	i_worlds.set_data("list_worlds", worldmap_names);
	i_worlds.select("list_worlds");
}

ScreenType WorldScreen::processInput()
{
	flushinp();
	long ch = getch();

	if (ch == 'c' || ch == 'C')
	{
		// Note: create_world();
		return ScreenType::CREATE_WORLD;
	}
	else if ((ch == 'd' || ch == 'D') && !worldmap_names.empty())
	{
		int index = i_worlds.get_int("list_worlds");
		std::string del_name = trim(worldmap_names[index]);

		if (showQueryYesNo(Doryen::format("Really delete {} and all saves?", del_name)))
		{
			worldmap_names.erase(worldmap_names.begin() + index);
			std::string dir_name = SAVE_DIR + "/" + del_name;
			std::string file_name = SAVE_DIR + "/worlds/" + del_name + ".map";

			if (directory_exists(dir_name) && !remove_directory(dir_name))
			{
				showDebugMessage(Doryen::format("Failed to remove directory '{}'.", dir_name));
			}

			if (!remove_file(file_name))
			{
				showDebugMessage(Doryen::format("Failed to remove file '{}'.", file_name));
			}

			i_worlds.set_data("list_worlds", worldmap_names);
			i_worlds.set_data("list_worlds", 0);
		}

		return ScreenType::NONE;
	}
	else if (ch == '\n')
	{
		indexWorldSelect = i_worlds.get_int("list_worlds");

		return ScreenType::MENU;
	}
	else
	{
		i_worlds.handle_action(ch); // Handles any scrolling
	}

	return ScreenType::NONE;
}
