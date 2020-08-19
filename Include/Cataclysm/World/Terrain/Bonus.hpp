// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_BONUS_HPP
#define CATACLYSM_BONUS_HPP

#include <Cataclysm/Enum/StatType.hpp>
#include <Cataclysm/Enum/MathOperator.hpp>

// For signal handling.
// TODO: Generalize this for entities & items, too?
struct Stat_bonus
{
	Stat_bonus(Stat_id _stat = STAT_NULL, Math_operator _op = MATH_NULL,
			int _amount = 0, int _static = 0) :
			stat(_stat), op(_op), amount(_amount), amount_static(_static)
	{
	}

	~Stat_bonus()
	{
	}

	bool load_data(std::istream& data, std::string owner_name);

	Stat_id stat;
	Math_operator op;
	int amount;
	int amount_static;  // Used if op is a comparison operator
};

#endif //CATACLYSM_BONUS_HPP
