// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_GENUS_HPP
#define CATACLYSM_GENUS_HPP

#include <string>
#include <vector>
#include "Cataclysm/Entity/Monster/Type.hpp"

struct Monster_genus
{
	Monster_genus();

	~Monster_genus();

	std::string name;
	std::string display_name;
	int uid;

	Monster_type default_values; // Default values for monsters in this genus
// Spawning stuff
	std::vector<Monster_type*> members;
	int total_chance;

	void add_member(Monster_type* member);

	Monster_type* random_member();

	void assign_uid(int id);

	std::string get_data_name();

	std::string get_name();

	bool load_data(std::istream& data);

};

#endif //CATACLYSM_GENUS_HPP
