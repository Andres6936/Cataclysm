// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_STATSELECTED_HPP
#define CATACLYSM_STATSELECTED_HPP

#include <cstdint>

namespace Cataclysm
{

	enum class Stat_selected : std::uint8_t
	{
		STATSEL_STR,
		STATSEL_DEX,
		STATSEL_PER,
		STATSEL_INT
	};

}

#endif //CATACLYSM_STATSELECTED_HPP
