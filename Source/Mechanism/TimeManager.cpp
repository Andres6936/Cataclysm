// Joan Andrés (@Andres6936) Github.

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

Time TimeManager::addHours(const std::int32_t _hours)
{
	return time + HOURS(_hours);
}

// Getters

Time& TimeManager::getTime() noexcept
{
	return time;
}
