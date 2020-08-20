// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Terrain/Bonus.hpp"
#include "Cataclysm/Screen/Debugger.hpp"
#include <istream>

// Stat_bonus loads from a single line, so it uses a different paradigm
bool Stat_bonus::load_data(std::istream& data, std::string owner_name)
{
// Get stat name
	if (!data.good())
	{
		debugmsg("Stat_bonus wasn't fed data (%s)", owner_name.c_str());
		return false;
	}
	std::string stat_name;
	data >> stat_name;
	stat = lookup_stat_id(stat_name);
	if (stat == STAT_NULL)
	{
		debugmsg("Unknown Stat_id name '%s' (%s)",
				stat_name.c_str(), owner_name.c_str());
		return false;
	}

// Get math symbol; valid options are * > >= < <= =
	if (!data.good())
	{
		debugmsg("Stat_bonus couldn't read operator (%s)", owner_name.c_str());
		return false;
	}
	std::string operator_name;
	data >> operator_name;
	op = lookup_math_operator(operator_name);
	if (op == MATH_NULL)
	{
		debugmsg("Unknown Math_operator '%s' (%s)",
				operator_name.c_str(), owner_name.c_str());
		return false;
	}
// Get amount
	if (!data.good())
	{
		debugmsg("Stat_bonus couldn't read amount (%s)", owner_name.c_str());
		return false;
	}
	data >> amount;

// If op is a comparison operator, we need the static amount
	if (op != MATH_MULTIPLY)
	{  // TODO: fix this if there's more non-comp ops
		if (!data.good())
		{
			debugmsg("Stat_bonus couldn't read static amount (%s)", owner_name.c_str());
			return false;
		}
		data >> amount_static;
	}
// Success!
	return true;
}

