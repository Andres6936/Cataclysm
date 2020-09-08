// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TIMEMANAGER_HPP
#define CATACLYSM_TIMEMANAGER_HPP

#include <cstdint>
#include <Cataclysm/Mechanism/time.h>

namespace Cataclysm
{

	class TimeManager
	{

	private:

		// Variables

		Time time {0, 0, 8, 1, SEASON_SPRING, STARTING_YEAR};

	public:

		// Methods

		bool turnTimer(const std::int32_t _turn);

		bool minuteTimer(const std::int32_t _minutes);

		Time addHours(const std::int32_t _hours);

		// Getters

		Time& getTime() noexcept;

	};

}

inline Cataclysm::TimeManager timeManager {};

#endif //CATACLYSM_TIMEMANAGER_HPP
