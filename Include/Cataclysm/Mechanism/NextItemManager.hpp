// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_NEXTITEMMANAGER_HPP
#define CATACLYSM_NEXTITEMMANAGER_HPP

#include <cstdint>
#include <zconf.h>

namespace Cataclysm
{

	class NextItemManager
	{

	private:

		std::int32_t nextItemUID = 0;

	public:

		// Getters

		const std::int32_t getItemUID() noexcept;

		const std::int32_t getNextItemUid() const noexcept;

	};

}

inline Cataclysm::NextItemManager nextItemManager {};

#endif //CATACLYSM_NEXTITEMMANAGER_HPP
