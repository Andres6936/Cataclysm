#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Util/files.h>

#include <Cataclysm/Util/Commnad/Parser.hpp>
#include <Cataclysm/Screen/ScreenManager.hpp>

#include <ctime>
#include <string>

const char* VERSION = "Cataclysm v2.0.0 alpha";

int main(int argc, char* argv[])
{
	Cataclysm::Parser parser {argc, argv};

	srand(time(NULL));  // Seed the RNG - TODO: Wrap this up
	init_display();     // See window.cpp

	//split_string("Strength affects the amount of weight you can carry or drag",
	//" \n", true);

	set_default_dirs(); // See files.cpp

	if (!prep_directories())
	{ // See globals.cpp - creates save dir & subdirs
		printf("Errors creating save directory structure.");
		return 1;
	}

	load_global_data(); // See globals.cpp

	Cataclysm::ScreenManager manager {};

	// Because ncurses blocks the thread until the user presses a key,
	// it is impossible for Cataclysm to draw the screen if the user does
	// not press a key before, this line of code will be deleted once Doryen
	// replaces completely to ncurses.
	manager.draw();

	while (manager.isRunning())
	{
		manager.processInput();
		manager.updated();
		manager.draw();
	}

	endwin();           // See window.cpp
	return 0;
}
