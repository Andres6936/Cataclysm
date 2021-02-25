// Joan Andrés (@Andres6936) Github.

#include <zconf.h>
#include "Cataclysm/Mechanism/TimeManager.hpp"

using namespace Cataclysm;

// Methods

bool TimeManager::turnTimer(const std::int32_t _turn)
{
	if (_turn <= 0)
	{
		return true;
	}
	else
	{
		return time.get_turn() % _turn == 0;
	}
}

bool TimeManager::minuteTimer(const std::int32_t _minutes)
{
	if (_minutes <= 0)
	{
		return true;
	}
	else
	{
		return turnTimer((_minutes * 60) / SECONDS_IN_TURN);
	}
}

const std::int32_t TimeManager::calculateLightLevel()
{
	const int ret = time.get_light_level();

	if (getLightLevel() > ret)
	{
		return getLightLevel();
	}
	else
	{
		return ret;
	}
}

Time TimeManager::addHours(const std::int32_t _hours)
{
	return time + HOURS(_hours);
}

// Getters

const std::int32_t TimeManager::getLightLevel() const noexcept
{
	return lightLevel;
}

Time& TimeManager::getTime() noexcept
{
	return time;
}

// Setters

void TimeManager::setLightLevel(const std::int32_t _lightLevel) noexcept
{
	lightLevel = _lightLevel;
}

void TimeManager::setCalculateLightLevel(const std::int32_t _lightLevel) noexcept
{
	if (_lightLevel > getLightLevel())
	{
		lightLevel = _lightLevel;
	}
}
