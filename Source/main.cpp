#include "Cataclysm/globals.h"
#include <Cataclysm/Util/files.h>

#include <Cataclysm/Screen/ScreenManager.hpp>

#include <ctime>
#include <getopt.h>
#include <string>

//#include "stringfunc.h"

const char* VERSION = "Cataclysm v2.0.0 alpha";

bool parse_options(int argc, char* argv[]);

void print_cli_help(std::string program_name);

void print_version();

bool prep_folders();

int main(int argc, char* argv[])
{
	TESTING_MODE = 0;
	if (!parse_options(argc, argv))
	{
		return 1;
	}

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

bool parse_options(int argc, char* argv[])
{
// extern variables used by getopt
	extern char* optarg;
	std::string argument;
	int c;
// TODO: Move these long options (and this function?) to a different file (?)
	static option long_options[] =
			{
// These options set a flag
					{ "test",     no_argument,       &TESTING_MODE, 1 },
					{ "debug",    no_argument,       &TESTING_MODE, 1 },
// These options do not set a flag.
					{ "data-dir", required_argument, 0,             'd' },
					{ "cuss-dir", required_argument, 0,             'c' },
					{ "save-dir", required_argument, 0,             's' },
					{ "help",     no_argument,       0,             'h' },
					{ "version",  no_argument,       0,             'v' },
					{ 0, 0,                          0,             0 }
			};
	int opt_index;

	while ((c = getopt_long(argc, argv, "", long_options, &opt_index)) != -1)
	{

		switch (c)
		{

		case 0:
// If the option set a flag, don't do anything else.
			break;

		case 'd': // --data-dir <directory>
			if (!set_dir(DATA_DIR, optarg))
			{
				printf("'%s' is not a valid data directory.\n", optarg);
				return false;
			}
			break;

		case 'c': // --cuss-dir <directory>
			if (!set_dir(CUSS_DIR, optarg))
			{
				printf("'%s' is not a valid cuss directory.\n", optarg);
				return false;
			}
			break;

		case 's': // --save-dir <directory>
			if (!set_dir(SAVE_DIR, optarg))
			{
				printf("'%s' is not a valid save directory.\n", optarg);
				return false;
			}
			break;

		case 'h':
			print_cli_help(argv[0]);
			return false; // Because we don't want to run the game.

		case 'v':
			print_version();
			return false; // Because we don't want to run the game.

		case '?':
// getopt_long handles printing these error messages.
			return false;

		}

	}

	return true;
}

/* I was going to make this load from a file, but I think that hardcoding it is
 * better practice.  We want it to work independently of the DATA_DIR setting,
 * or of the presence of any files.
 */
void print_cli_help(std::string program_name)
{
	print_version();
	printf("\n\
Usage: %s [options]\n\
\n\
Options:\n\
  --test                    Enable test mode (cheat commands, increased info)\n\
  --debug                   Enable debug mode (for now, identical to --test)\n\
  --data-dir <directory>    Use <directory> as a source for game data\n\
  --cuss-dir <directory>    Use <directory> as a source for interface data\n\
  --help                    Print this help message and exit.\n\
  --version                 Print the version and exit.\n",
			program_name.c_str());
}

void print_version()
{
	printf("%s\n", VERSION);
}

