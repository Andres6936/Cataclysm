// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Entity/Plan.hpp"
#include <Cataclysm/Util/globals.h>

Entity_plan::Entity_plan()
{
	target_point = Tripoint(-1, -1, -1);
	target_entity = NULL;
	attention = 0;
	goal_type = AIGOAL_NULL;
}

Entity_plan::~Entity_plan()
{
}

void Entity_plan::set_target(AI_goal goal, Tripoint target, int att)
{
	if (att == -1)
	{ // att defaults to -1
		att = 15;
	}
	target_point = target;
	target_entity = NULL; // TODO: Don't do this?
	attention = att;
	goal_type = goal;
}

void Entity_plan::set_target(AI_goal goal, Entity* target, int att)
{
	if (!target)
	{
		target_point = Tripoint(-1, -1, -1);
		attention = 0;
		target_entity = NULL;
		return;
	}
	if (att == -1)
	{ // att defaults to -1
		att = 15;
	}
	target_entity = target;
	target_point = target->pos;
	attention = att;
	goal_type = goal;
}

void Entity_plan::generate_path_to_target(Entity_AI AI, Tripoint origin)
{
	if (!is_active() || target_point.x < 0 || target_point.y < 0)
	{
		return;
	}

	Generic_map map = GAME.map->get_movement_map(AI, origin, target_point);
	Pathfinder pf(map);

	path = pf.get_path(PATH_A_STAR, origin, target_point);

	if (path.empty())
	{
		attention = 0;
		target_point = Tripoint(-1, -1, -1);
		target_entity = NULL;
	}
}

void Entity_plan::update()
{
	if (attention > 0)
	{
		attention--;
	}
	if (attention <= 0)
	{
		target_entity = NULL;
	}
}

bool Entity_plan::is_active()
{
	if (attention <= 0)
	{
		return false;
	}
	if (target_entity)
	{
		return true;
	}
	if (target_point.x < 0 || target_point.y < 0)
	{
		return false;
	}
	return true;
}

Tripoint Entity_plan::get_target()
{
	if (target_point.x >= 0 && target_point.y >= 0)
	{
		return target_point;
	}
	if (target_entity)
	{
		return target_entity->pos;
	}
	return Tripoint(-1, -1, -1);
}

Tripoint Entity_plan::next_step()
{
	if (path.empty())
	{
		return Tripoint(-1, -1, -1);
	}
	return path[0];
}

void Entity_plan::erase_step()
{
	path.erase_step(0);
}

void Entity_plan::clear()
{
	target_point = Tripoint(-1, -1, -1);
	target_entity = NULL;
	attention = 0;
	goal_type = AIGOAL_NULL;
	path.clear();
}

void Entity_plan::shift(int shiftx, int shifty)
{
	if (target_point.x > -1)
	{
		target_point.x -= shiftx * SUBMAP_SIZE;
		target_point.y -= shifty * SUBMAP_SIZE;
	}
	if (target_point.x < 0 || target_point.y < 0)
	{
		clear();
	}
	path.shift(shiftx, shifty);
}

