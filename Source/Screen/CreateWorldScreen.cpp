// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/CreateWorldScreen.hpp"
#include <Cataclysm/Util/files.h>

using namespace Cataclysm;

// Construct

CreateWorldScreen::CreateWorldScreen()
{
	if (!i_editor.load_from_file(CUSS_DIR + "/i_world_editor.cuss"))
	{
		throw std::runtime_error("Cannot found the file i_world_editor.cuss");
	}

	i_editor.ref_data("entry_name", &world_name);
	i_editor.select("entry_name");
}

// Methods

void CreateWorldScreen::draw()
{
	i_editor.draw(createWorldConsole);

	createWorldConsole.blit({0, 0}, console, {0, 0});
	createWorldConsole.draw();
}

void CreateWorldScreen::updated()
{

}

ScreenType CreateWorldScreen::processInput()
{
	flushinp();
	long ch = getch();

	if (ch == '!')
	{
		tmp_world.randomize_name();
		world_name = tmp_world.get_name();
	}
	else if (ch == '\n')
	{
		if (tmp_world.get_name().empty())
		{
			showDebugMessage("Name is empty!");
		}
		else
		{
			console.writeString(1, 1, "Generating world, please wait...");

			tmp_world.generate();

			if (tmp_world.save_to_name())
			{
				worldmap_names.push_back(tmp_world.get_name());
			}
		}

		return ScreenType::WORLD;
	}
	else if (ch == KEY_ESC)
	{
		return ScreenType::WORLD;

	}
	else if (ch == '@')
	{
		// TODO: Unlock advanced options.
	}
	else
	{
		i_editor.handle_keypress(ch);
		tmp_world.set_name(world_name);
	}

	return ScreenType::NONE;
}
