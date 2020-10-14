// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_UTILITY_HPP
#define CATACLYSM_UTILITY_HPP

#include <string>
#include <vector>
#include <Doryen/Doryen.hpp>
#include <Cuss/Graphics/color.h>

namespace cuss
{

	class Utility
	{

	public:

		/**
		 * Remove the color tags of text.
		 *
		 * @param _text Text to delete the color tags
		 * @return The text without color tags.
		 */
		static std::string removeColorTags(std::string_view _text);

		static std::vector<std::string> break_into_lines(
				const std::string& text, int linesize);

		static std::string load_to_delim(std::istream& datastream,
				std::string delim);

		static std::string stripTags(std::string _text);

		static std::vector<std::pair<Doryen::Color, Doryen::Color>> parseColorTags(
				std::string text, std::vector<std::string>& segments,
				std::vector<long>& color_pairs, nc_color fg, nc_color bg);

	};

}

#endif //CATACLYSM_UTILITY_HPP
