// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Entity/Stats.hpp"


Stats::Stats()
{
	strength = 0;
	dexterity = 0;
	intelligence = 0;
	perception = 0;
}

Stats::~Stats()
{
}

void Stats::reset()
{
	strength = strength_max;
	dexterity = dexterity_max;
	intelligence = intelligence_max;
	perception = perception_max;
}

Stats& Stats::operator=(const Stats& rhs)
{
	strength = rhs.strength;
	dexterity = rhs.dexterity;
	intelligence = rhs.intelligence;
	perception = rhs.perception;
	return *this;
}

Stats& Stats::operator+=(const Stats& rhs)
{
	strength += rhs.strength;
	dexterity += rhs.dexterity;
	intelligence += rhs.intelligence;
	perception += rhs.perception;
	return *this;
}

Stats& Stats::operator-=(const Stats& rhs)
{
	strength -= rhs.strength;
	dexterity -= rhs.dexterity;
	intelligence -= rhs.intelligence;
	perception -= rhs.perception;
	return *this;
}

