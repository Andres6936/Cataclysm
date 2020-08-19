// Joan Andrés (@Andres6936) Github.

#include <sstream>

#include "Cataclysm/Attack/Verbs.hpp"
#include "Cataclysm/window.h" // For debugmsg

bool load_verbs(std::istream& data, std::string& verb_second,
		std::string& verb_third, std::string owner_name)
{
	std::string verb_line;
	std::getline(data, verb_line);
	std::istringstream verb_data(verb_line);

	std::string tmpword;
	std::string tmpverb;
	bool reading_second = true;

	while (verb_data >> tmpword)
	{
		if (tmpword == "/")
		{
			if (!reading_second)
			{
				debugmsg("Too many / in verb definition (%s)", owner_name.c_str());
				return false;
			}
			reading_second = false;
			verb_second = tmpverb;
			tmpverb = "";
		}
		else
		{
			if (!tmpverb.empty())
			{
				tmpverb += " ";
			}
			tmpverb += tmpword;
		}
	}

	if (reading_second)
	{
		debugmsg("Verb definition has 2nd person, but no 3rd", owner_name.c_str());
		return false;
	}
	verb_third = tmpverb;

	return true;
}


