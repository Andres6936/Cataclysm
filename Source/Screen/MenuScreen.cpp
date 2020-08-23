// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/MenuScreen.hpp"
#include <Cataclysm/Screen/Debugger.hpp>
#include <Cataclysm/files.h>
#include <Cuss/Element.hpp>


using namespace Cataclysm;

// Construct

MenuScreen::MenuScreen()
{
	if (!i_hud.load_from_file(CUSS_DIR + "/i_hud.cuss"))
	{
		throw std::runtime_error("Not is possible found i_hud.cuss");
	}

	int xdim, ydim;
	get_screen_dims(xdim, ydim);
	int win_size = ydim;

	//Commenting this out for now - the extra empty space caused issues
	//if (win_size % 2 == 0) {
	//	win_size--; // Only odd numbers allowed!
	//}

	w_map = std::make_unique<Window>(0, 0, win_size, win_size);
	w_hud = std::make_unique<Window>(win_size, 0, xdim - win_size, ydim);

	// Attempt to resize the messages box to be as tall as the window allows
	cuss::element* messages = i_hud.select("text_messages");

	if (!messages)
	{
		throw std::runtime_error("Couldn't find element text_messages in i_hud");
	}

	messages->sizey = ydim - messages->posy;

	// Populate Worldmap_names with all the Worldmaps in SAVE_DIR/worlds
	worldmap_names = files_in(SAVE_DIR + "/worlds", ".map");

	// Strip the ".map" from each Worldmap name
	for (int i = 0; i < worldmap_names.size(); i++)
	{
		size_t suffix = worldmap_names[i].find(".map");
		if (suffix != std::string::npos)
		{
			worldmap_names[i] = worldmap_names[i].substr(0, suffix);
		}
	}

	if (!i_menu.load_from_file(CUSS_DIR + "/i_starting_menu.cuss"))
	{
		throw std::runtime_error("Cannot found i_starting_menu.cuss");
	}

	std::string motd = slurp_file(DATA_DIR + "/motd.txt");

	i_menu.set_data("text_motd", motd);
}

// Methods

void MenuScreen::draw()
{
	i_menu.draw(w_menu);
	i_menu.draw(screenMenu);

	w_menu->refresh();

	screenMenu.blit({0, 0}, console, {0, 0});
	screenMenu.draw();
}

void MenuScreen::updated()
{

}

ScreenType MenuScreen::processInput()
{
	long ch = input();

	if (ch == 'n' || ch == 'N')
	{
//		if (setup_new_game(current_world))
//		{
//			return true;
//		}
		return ScreenType::NONE;
	}
	else if (ch == 'l' || ch == 'L')
	{
		// TODO: Load character here
		return ScreenType::NONE;
	}
	else if (ch == 'w' || ch == 'W')
	{
//		current_world = world_screen();
		return ScreenType::NONE;
	}
	else if (ch == 'h' || ch == 'H')
	{
		return ScreenType::HELP_MENU;
	}
	else if (ch == 'q' || ch == 'Q')
	{
		return ScreenType::QUIT;
	}

	return ScreenType::NONE;
}
