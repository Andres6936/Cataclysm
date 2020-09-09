// Joan Andrés (@Andres6936) Github.

#include <vector>
#include <stdexcept>
#include "Cataclysm/Util/Commnad/Parser.hpp"

using namespace Cataclysm;

Parser::Parser(const std::uint8_t argc, char* argv[])
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
}

void Parser::parserArguments() const noexcept
{
	for (const auto& [key, value] : *this)
	{

	}
}
