// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_UTILITY_HPP
#define CATACLYSM_UTILITY_HPP

#include <string>
#include <vector>
#include <Doryen/Doryen.hpp>
#include <Cataclysm/color.h>

namespace cuss
{

	class Utility
	{

	public:

		static std::string stripTags(std::string _text);

		static std::vector<std::pair<Doryen::Color, Doryen::Color>> parseColorTags(
				std::string text, std::vector<std::string>& segments,
				std::vector<long>& color_pairs, nc_color fg, nc_color bg);

	};

}

#endif //CATACLYSM_UTILITY_HPP
