// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_PLAN_HPP
#define CATACLYSM_PLAN_HPP


#include <Cataclysm/entity_ai.h>
#include <Cataclysm/pathfind.h>

class Entity;

struct Entity_plan
{
	Entity_plan();

	~Entity_plan();

	void set_target(AI_goal goal, Tripoint target, int att = -1);

	void set_target(AI_goal goal, Entity* target, int att = -1);

	void generate_path_to_target(Entity_AI AI, Tripoint origin);

	void update();  // Decrement attention, reset target_entity if <= 0

	bool is_active();

	Tripoint get_target();

	Tripoint next_step();

	void erase_step();

	void clear();

	void shift(int shiftx, int shifty);

	Tripoint target_point;
	Entity* target_entity;
	Pathfinder pf;
	Path path;
	int attention;
	AI_goal goal_type;
};

#endif //CATACLYSM_PLAN_HPP
