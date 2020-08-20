// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_STATS_HPP
#define CATACLYSM_STATS_HPP

struct Stats
{
	Stats();

	Stats(int S, int D, int I, int P) :
			strength(S), dexterity(D), intelligence(I), perception(P)
	{
	}

	~Stats();

	void reset(); // Set SDIP to maxes

	Stats& operator=(const Stats& rhs);

	Stats& operator+=(const Stats& rhs);

	Stats& operator-=(const Stats& rhs);

	int strength, dexterity, intelligence, perception;
	int strength_max, dexterity_max, intelligence_max, perception_max;
};

inline Stats operator+(Stats lhs, const Stats& rhs)
{
	lhs += rhs;
	return lhs;
}

inline Stats operator-(Stats lhs, const Stats& rhs)
{
	lhs -= rhs;
	return lhs;
}

#endif //CATACLYSM_STATS_HPP
