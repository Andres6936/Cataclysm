// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TIMEMANAGER_HPP
#define CATACLYSM_TIMEMANAGER_HPP

#include <cstdint>
#include <Cataclysm/Mechanism/time.h>
#include <zconf.h>

namespace Cataclysm
{

	class TimeManager
	{

	private:

		// Variables

		std::int32_t lightLevel = 0;

		Time time {0, 0, 8, 1, SEASON_SPRING, STARTING_YEAR};

	public:

		// Methods

		/**
		 * @return True once every turns.
		 */
		bool turnTimer(const std::int32_t _turn);

		/**
		 * @return True once every minutes.
		 */
		bool minuteTimer(const std::int32_t _minutes);

		const std::int32_t calculateLightLevel();

		Time addHours(const std::int32_t _hours);

		// Getters

		const std::int32_t getLightLevel() const noexcept;

		Time& getTime() noexcept;

		// Setters

		void setLightLevel(const std::int32_t _lightLevel) noexcept;

		void setCalculateLightLevel(const std::int32_t _lightLevel) noexcept;

	};

}

inline Cataclysm::TimeManager timeManager {};

#endif //CATACLYSM_TIMEMANAGER_HPP
