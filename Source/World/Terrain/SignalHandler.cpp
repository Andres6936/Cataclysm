// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/World/Terrain/SignalHandler.hpp"
#include <Cataclysm/Screen/Debugger.hpp>
#include <Cataclysm/Util/String/stringfunc.h>

Terrain_signal_handler::Terrain_signal_handler()
{
	success_rate = 100;
}


bool Terrain_signal_handler::load_data(std::istream& data,
		std::string owner_name)
{
	std::string ident, junk;
	while (ident != "done" && !data.eof())
	{
		if (!(data >> ident))
		{
			return false;
		}
		ident = no_caps(ident);

		if (!ident.empty() && ident[0] == '#')
		{
// It's a comment
			std::getline(data, junk);

		}
		else if (ident == "result:")
		{
			std::getline(data, result);
			result = trim(result);

		}
		else if (ident == "success_rate:")
		{
			data >> success_rate;
			std::getline(data, junk);

		}
		else if (ident == "success_message:")
		{
			std::getline(data, success_message);
			success_message = trim(success_message);

		}
		else if (ident == "failure_message:")
		{
			std::getline(data, failure_message);
			failure_message = trim(failure_message);

		}
		else if (ident == "stat_bonus:")
		{
			std::string bonus_text;
			std::getline(data, bonus_text);
			std::istringstream bonus_data(bonus_text);
			Stat_bonus tmp;
			if (!tmp.load_data(bonus_data, owner_name + " signal handler"))
			{
				return false;
			}
			stat_bonuses.push_back(tmp);

		}
		else if (ident == "terrain_flag_bonus:")
		{
			std::string bonus_text;
			std::getline(data, bonus_text);
			std::istringstream bonus_data(bonus_text);
			Terrain_flag_bonus tmp;
			if (!tmp.load_data(bonus_data, owner_name + " signal handler"))
			{
				return false;
			}
			terrain_flag_bonuses.push_back(tmp);

		}
		else if (ident != "done")
		{
			debugmsg("Unknown terrain property '%s' (%s)",
					ident.c_str(), owner_name.c_str());
		}
	}
	return true;
}