// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_NEWGAMESCREENTYPE_HPP
#define CATACLYSM_NEWGAMESCREENTYPE_HPP

#include <cstdint>

namespace Cataclysm
{

	enum class New_char_screen : std::uint8_t
	{
		NCS_CANCEL,
		NCS_STATS,
		NCS_TRAITS,
		NCS_PROFESSION,
		NCS_DESCRIPTION,
		NCS_DONE
	};

}

#endif //CATACLYSM_NEWGAMESCREENTYPE_HPP
