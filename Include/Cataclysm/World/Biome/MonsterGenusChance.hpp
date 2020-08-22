// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_GENUSCHANCE_HPP
#define CATACLYSM_GENUSCHANCE_HPP

// TODO: Move this to monster_type.h?
struct Monster_genus_chance
{
	Monster_genus_chance(int C = 100, Monster_genus* MG = NULL) :
			chance(C), genus(MG)
	{
	}

	int chance;
	Monster_genus* genus;
};

#endif //CATACLYSM_GENUSCHANCE_HPP
