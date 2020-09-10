// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Enum/MathOperator.hpp"
#include <Cataclysm/Util/String.hpp>

Math_operator lookup_math_operator(std::string name)
{
// Don't need to no_caps name since it doesn't contain letters
	name = trim(name);
	for (int i = 0; i < MATH_MAX; i++)
	{
		Math_operator ret = Math_operator(i);
		if (math_operator_name(ret) == name)
		{
			return ret;
		}
	}
	return MATH_NULL;
}

std::string math_operator_name(Math_operator op)
{
	switch (op)
	{
	case MATH_NULL:
		return "NULL";
	case MATH_MULTIPLY:
		return "*";
	case MATH_GREATER_THAN:
		return ">";
	case MATH_GREATER_THAN_OR_EQUAL_TO:
		return ">=";
	case MATH_LESS_THAN:
		return "<";
	case MATH_LESS_THAN_OR_EQUAL_TO:
		return "<=";
	case MATH_EQUAL_TO:
		return "=";
	case MATH_MAX:
		return "BUG - MATH_MAX";
	default:
		return "BUG - Unnamed Math_operator";
	}
	return "BUG - Escaped math_operator_name() switch";
}


