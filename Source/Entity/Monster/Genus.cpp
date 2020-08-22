// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Entity/Monster/Genus.hpp"
#include <Cataclysm/Screen/Debugger.hpp>
#include "Cataclysm/stringfunc.h"
#include "Cataclysm/Random/rng.h"

Monster_genus::Monster_genus()
{
	uid = -1;
	total_chance = 0;
}

Monster_genus::~Monster_genus()
{
}

void Monster_genus::add_member(Monster_type* member)
{
	if (!member)
	{
		return;
	}
	total_chance += member->chance;
	members.push_back(member);
}

Monster_type* Monster_genus::random_member()
{
	if (members.empty())
	{
		return NULL;
	}
	int index = rng(1, total_chance);
	for (int i = 0; i < members.size(); i++)
	{
		index -= members[i]->chance;
		if (index <= 0)
		{
			return members[i];
		}
	}
	return members.back();
}

void Monster_genus::assign_uid(int id)
{
	uid = id;
}

std::string Monster_genus::get_data_name()
{
	return name;
}

std::string Monster_genus::get_name()
{
	if (display_name.empty())
	{
		return name;
	}
	return display_name;
}

bool Monster_genus::load_data(std::istream& data)
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
		else if (ident == "name:")
		{
			std::getline(data, name);
			name = trim(name);

		}
		else if (ident == "display_name:")
		{
			std::getline(data, display_name);
			display_name = trim(display_name);

		}
		else if (ident == "default:")
		{
			if (!default_values.load_data(data))
			{
				debugmsg("Genus %s failed to load default values", name.c_str());
				return false;
			}

		}
		else if (ident != "done")
		{
			debugmsg("Unknown monster genus property '%s' (%s)",
					ident.c_str(), name.c_str());
			return false;
		}
	}
	return true;
}

