// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_NEXTFURNITUREMANAGER_HPP
#define CATACLYSM_NEXTFURNITUREMANAGER_HPP

#include <cstdint>
#include <zconf.h>

namespace Cataclysm
{

	class NextFurnitureManager
	{

		std::int32_t nextFurniture = 0;

	public:

		const std::int32_t getNextFurniture() noexcept;

	};

}

inline Cataclysm::NextFurnitureManager nextFurnitureManager {};

#endif //CATACLYSM_NEXTFURNITUREMANAGER_HPP
