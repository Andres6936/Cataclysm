// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_MATHOPERATOR_HPP
#define CATACLYSM_MATHOPERATOR_HPP

#include <string>

// These are used, for now, in signal_handlers
// TODO: Expand this?  It's simple right now...
enum Math_operator
{
	MATH_NULL = 0,
	MATH_MULTIPLY,
	MATH_GREATER_THAN,
	MATH_GREATER_THAN_OR_EQUAL_TO,
	MATH_LESS_THAN,
	MATH_LESS_THAN_OR_EQUAL_TO,
	MATH_EQUAL_TO,
	MATH_MAX
};

Math_operator lookup_math_operator(std::string name);

std::string math_operator_name(Math_operator op);

#endif //CATACLYSM_MATHOPERATOR_HPP
