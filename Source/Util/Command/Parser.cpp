// Joan Andrés (@Andres6936) Github.

#include <vector>
#include <iostream>
#include <stdexcept>
#include <Cataclysm/Util/files.h>
#include "Cataclysm/Util/Commnad/Parser.hpp"

using namespace Cataclysm;

Parser::Parser(const std::int32_t argc, char* argv[])
{
	// The first argument is the name of program,
	// if only 1 argument exist, exit of construct
	if (argc == 1) return;

	std::vector<std::string> arguments;

	// Begin from 1, skip the argument that
	// had the name of program
	for (int i = 1; i < argc; ++i)
	{
		arguments.push_back(argv[i]);
	}

	// The arguments should be pares: (key, value)
	if (arguments.size() % 2 == 0)
	{
		for(int j = 0; j < arguments.size(); ++j)
		{
			try
			{
				// Key and Value
				insert({arguments.at(j), arguments.at(j + 1)});
			}
				// Is possible that try access to element j + 1 the
				// vector throw a exception
			catch (std::out_of_range& exception)
			{
				throw std::invalid_argument("The arguments " + arguments.at(j) + " not have a value");
			}
		}
	}
	else
	{
		// The arguments not satisfies the invariants: (key, value)
		throw std::invalid_argument("The arguments should be pares: (key, value)");
	}

	parserArguments();
}

void Parser::parserArguments() const noexcept
{
	for (const auto& [key, value] : *this)
	{
		if (key == "--data.directory:")
		{
			if (not set_dir(DATA_DIR, value))
			{
				std::cerr << "The directory " << value << " not exist\n";
			}
		}
		else if (key == "--cuss.directory:")
		{
			if (not set_dir(CUSS_DIR, value))
			{
				std::cerr << "The directory " << value << " not exist\n";
			}
		}
		else if (key == "--save.directory:")
		{
			if (not set_dir(SAVE_DIR, value))
			{
				std::cerr << "The directory " << value << " not exist\n";
			}
		}
		else if (key == "--help")
		{
			printMessageHelp();
		}
		else if (key == "--version")
		{
			printVersion();
		}
		else if (key == "--debug:")
		{

		}
	}
}

void Parser::printVersion() const noexcept
{
	std::cout << "Cataclysm September 2020\n";
}

void Parser::printMessageHelp() const noexcept
{
	std::cout << "\n\n";
	std::cout << "--debug: <ON/OFF>         Enable debug mode (for now, identical to --test)";
	std::cout << "--data.directory: <directory>    Use <directory> as a source for game data";
	std::cout << "--cuss.directory: <directory>    Use <directory> as a source for interface data";
	std::cout << "--help                    Print this help message and exit.";
	std::cout << "--version                 Print the version and exit.";
	std::cout << "\n\n";
}
