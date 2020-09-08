// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Entity/Pool.hpp>
#include <Cataclysm/Entity/Player/player.h>
#include <Cataclysm/Mechanism/TimeManager.hpp>
#include "Cataclysm/Mechanism/TargetSelectorManager.hpp"

using namespace Cataclysm;

std::vector<Tripoint>
TargetSelectorManager::pathSelector(int startx, int starty, int range, bool target_entities, bool show_path)
{
	std::vector<Tripoint> ret;
	if (!player)
	{
		return ret;
	}
	if (startx == -1 || starty == -1)
	{
		startx = player->pos.x;
		starty = player->pos.y;
	}

	int minx, miny, maxx, maxy;
	if (range == -1)
	{  // Range defaults to -1, "no limit"
		range = getLevelLight();
	}
	if (range > getLevelLight())
	{
		range = getLevelLight();
	}
	minx = startx - range;
	miny = starty - range;
	maxx = startx + range;
	maxy = starty + range;

	Tripoint target(startx, starty, player->pos.z);
	std::vector<Tripoint> target_list;
	std::vector<int> target_range;
	int target_index = 0;
	if (target_entities)
	{
// Set up list of targets.
		for (std::list<Entity*>::iterator it = entities.instances.begin();
			 it != entities.instances.end();
			 it++)
		{
			Entity* ent_target = (*it);
			int ent_range = rl_dist(player->pos, ent_target->pos);
			if (ent_target != player.get() && ent_range <= range &&
				player->is_enemy(ent_target) && player->can_sense(ent_target))
			{
// They should be included; figure out where to place them in the list
				bool found = false;
				for (int i = 0; !found && i < target_range.size(); i++)
				{
					if (target_range[i] > ent_range)
					{
						found = true;
						target_range.insert(target_range.begin() + i, ent_range);
						target_list.insert(target_list.begin() + i, ent_target->pos);
					}
				}
				if (!found)
				{ // Stick them on the end
					target_range.push_back(ent_range);
					target_list.push_back(ent_target->pos);
				}
			}
		}
		if (last_target == -1)
		{  // No previous target to snap to, pick the closest
			Entity* new_target = entities.closest_seen_by(player.get(), range);
			if (new_target)
			{ // It'll be NULL if no one is in range
				target = new_target->pos;
				bool found = false;
				for (int i = 0; !found && i < target_list.size(); i++)
				{
					if (target == target_list[i])
					{
						found = true;
						target_index = i;
					}
				}
			}
		}
		else
		{
			Entity* old_target = entities.lookup_uid(last_target);
// It'll be NULL if the old target's dead, etc.
			if (old_target && player->can_sense(old_target))
			{
				target = old_target->pos;
				bool found = false;
				for (int i = 0; !found && i < target_list.size(); i++)
				{
					if (target == target_list[i])
					{
						found = true;
						target_index = i;
					}
				}
			}
			else
			{
// Reset last_target
				last_target = -1;
				Entity* new_target = entities.closest_seen_by(player.get(), range);
				if (new_target)
				{ // It'll be NULL if no one is in range
					target = new_target->pos;
					bool found = false;
					for (int i = 0; !found && i < target_list.size(); i++)
					{
						if (target == target_list[i])
						{
							found = true;
							target_index = i;
						}
					}
				}
			}
		}
	}
//
//// First draw; we need to draw the path since we might auto-target
//	ret = map->line_of_sight(player->pos, target);
//	map->draw_area(w_map, &entities, player->pos, minx, miny, maxx, maxy);
//	if (show_path)
//	{
//		for (int i = 0; i < ret.size(); i++)
//		{
//			map->draw_tile(w_map, &entities, ret[i].x, ret[i].y,
//					player->pos.x, player->pos.y, true); // true==inverted
//		}
//	}
//	else
//	{
//// Just do the last tile
//		map->draw_tile(w_map, &entities, ret.back().x, ret.back().y,
//				player->pos.x, player->pos.y, true);
//	}
//
//// TODO: No no no remove this!  Won't work for tiles!
//	Entity* ent_targeted = entities.entity_at(target);
//	if (ent_targeted)
//	{
//		w_map->putglyph(w_map->sizex() / 2 - player->pos.x + target.x,
//				w_map->sizey() / 2 - player->pos.y + target.y,
//				ent_targeted->get_glyph().invert());
//	}
//	else
//	{
//		w_map->putglyph(w_map->sizex() / 2 - player->pos.x + target.x,
//				w_map->sizey() / 2 - player->pos.y + target.y,
//				glyph('*', c_red, c_black));
//	}
//	w_map->refresh();
//
//	while (true)
//	{
//		long ch = input();
//		bool redraw = false;
//		if (ch == KEY_ESC || ch == 'q' || ch == 'Q')
//		{
//			return std::vector<Tripoint>();
//
//		}
//		else if ((ch == '<' || ch == '-') && !target_list.empty())
//		{
//			target_index--;
//			if (target_index < 0)
//			{
//				target_index = target_list.size() - 1;
//			}
//			target = target_list[target_index];
//			redraw = true;
//
//		}
//		else if ((ch == '>' || ch == '+' || ch == '=') && !target_list.empty())
//		{
//			target_index++;
//			if (target_index >= target_list.size())
//			{
//				target_index = 0;
//			}
//			target = target_list[target_index];
//			redraw = true;
//
//		}
//		else if (ch == '\n')
//		{
//			return ret;
//
//		}
//		else
//		{
//			Point p = input_direction(ch);
//			if (p.x == 0 && p.y == 0)
//			{
//				return ret; // Return our path on hitting "pause"
//			}
//			else if (p.x != -2 && p.y != -2)
//			{
//				target += p;
//// Ensure we're still in-range
//				if (target.x < minx)
//				{
//					target.x = minx;
//				}
//				if (target.y < miny)
//				{
//					target.y = miny;
//				}
//				if (target.x > maxx)
//				{
//					target.x = maxx;
//				}
//				if (target.y > maxy)
//				{
//					target.y = maxy;
//				}
//				redraw = true;
//			}
//		}
//
//		if (redraw)
//		{
//			ret = map->line_of_sight(player->pos, target);
//			map->draw_area(w_map, &entities, player->pos, minx, miny, maxx, maxy);
//			if (show_path)
//			{
//				for (int i = 0; i < ret.size(); i++)
//				{
//					map->draw_tile(w_map, &entities, ret[i].x, ret[i].y,
//							player->pos.x, player->pos.y, true);// true==inverted
//				}
//			}
//// TODO: No no no remove this!  Won't work for tiles!
//			ent_targeted = entities.entity_at(target);
//			if (ent_targeted)
//			{
//				w_map->putglyph(w_map->sizex() / 2 - player->pos.x + target.x,
//						w_map->sizey() / 2 - player->pos.y + target.y,
//						ent_targeted->get_glyph().invert());
//			}
//			else
//			{
//				w_map->putglyph(w_map->sizex() / 2 - player->pos.x + target.x,
//						w_map->sizey() / 2 - player->pos.y + target.y,
//						glyph('*', c_red, c_black));
//			}
//			w_map->refresh();
//		}
//	}
}

Tripoint TargetSelectorManager::select(int startx, int starty, int range, bool target_entities, bool show_path)
{
	std::vector<Tripoint> path = pathSelector(startx, starty, range, target_entities, show_path);

	if (path.empty())
	{
		return Tripoint(-1, -1, -1);
	}

	return path.back();
}

std::int32_t TargetSelectorManager::getLevelLight()
{
	int ret = timeManager.getTime().get_light_level();
	if (timeManager.getLightLevel() > ret)
	{
		return timeManager.getLightLevel();
	}
	return ret;
}
