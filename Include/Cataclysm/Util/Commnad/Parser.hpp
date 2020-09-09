// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_PARSER_HPP
#define CATACLYSM_PARSER_HPP

#include <map>
#include <string>
#include <cstdint>

namespace Cataclysm
{

	class Parser : private std::map<std::string, std::string>
	{

	private:

		void parserArguments() const noexcept;

	public:

		Parser(const std::uint8_t argc, char* argv[]);

	};

}

#endif //CATACLYSM_PARSER_HPP
