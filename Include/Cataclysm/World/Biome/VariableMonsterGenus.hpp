// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_VARIABLEMONSTERGENUS_HPP
#define CATACLYSM_VARIABLEMONSTERGENUS_HPP

#include <vector>
#include <istream>
#include <Cataclysm/Entity/Monster/Genus.hpp>
#include "Cataclysm/World/Biome/MonsterGenusChance.hpp"

struct Variable_monster_genus
{
public:
	Variable_monster_genus();

	~Variable_monster_genus();

	void add_genus(int chance, Monster_genus* genus);

	void add_genus(Monster_genus_chance genus);

	bool load_data(std::istream& data, std::string name = "unknown");

	int size();

	Monster_genus* pick();  // Pick a single random genus
	std::vector<Monster_genus*> pick(int num);  // Return a list of $num genera
	int pick_number(); // Pick how many genera to spawn

private:
	std::vector<Monster_genus_chance> genera;
	int total_chance;
};

#endif //CATACLYSM_VARIABLEMONSTERGENUS_HPP
