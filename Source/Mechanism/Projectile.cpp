// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Random/rng.h>
#include <Cataclysm/Entity/Pool.hpp>
#include <Cataclysm/Item/TypeLauncher.hpp>
#include <Cataclysm/Entity/Player/player.h>
#include <Cataclysm/Util/String.hpp>
#include "Cataclysm/Mechanism/Projectile.hpp"
#include <Cataclysm/Visual/Screen/MessageQueue.hpp>

using namespace Cataclysm;

void Projectile::launch(Entity* shooter, Item it, Ranged_attack attack, Tripoint origin, Tripoint target)
{
	// Set up some nouns and verbs for messages (far below)
	std::string shooter_name, verb = "shoot", miss_verb = "miss",
			graze_verb = "graze";
	if (shooter)
	{
		shooter_name = shooter->get_name_to_player();
		verb = shooter->conjugate(verb);
		miss_verb = shooter->conjugate(miss_verb);
		graze_verb = shooter->conjugate(graze_verb);
	}
	else
	{
/* If there's no shooter, that implies that natural forces launched the
 * projectile, e.g. rubble from an explosion.  In that case, we want our hit
 * message to be "A piece of rubble hits you!"
 */
		if (it.is_real())
		{
			shooter_name = it.get_name_indefinite();
		}
		verb = "hits";
		miss_verb = "misses";
		graze_verb = "grazes";
	}

// We want to loop through once for each round.  Also, (target) will probably
// change between rounds, so we recalculate range etc.
	int retarget_range = 0;
	if (attack.rounds < 1)
	{
		attack.rounds = 1;
	}
	else if (it.is_real())
	{
/* If we're firing a multiple-round weapon, and we kill our original target or
 * wind up targeting a space without a monster in it, we can re-target to a
 * nearby monster between rounds.  The range of this retargeting is dependent
 * upon our skills.
 */
		Item_type_launcher* launcher_type = static_cast<Item_type_launcher*>(it.get_type());
		Skill_type launcher_skill = launcher_type->skill_used;
		if (shooter)
		{
			retarget_range = shooter->skills.get_level(launcher_skill) +
							 shooter->skills.get_level(SKILL_LAUNCHERS) / 2;
			retarget_range = sqrt(retarget_range);
		}
	}

/* If we're NOT targeting an entity, then we are probably shooting at scenery
 * and we should not use our retargeting ability!
 */
	bool targeting_entity = entities.entity_at(target) not_eq nullptr;

	for (int round = 0; round < attack.rounds; round++)
	{
// Figure out the range to the target
		int range = rl_dist(origin, target);

// We calculate angle individually for every pellet...
		int num_pellets = attack.pellets;
		if (num_pellets < 1)
		{
			num_pellets = 1;
		}
/* We summarize all pellets in a single message to avoid spamming the player
 * with 20 messages for a single round of 00 shot.  We do this with a few
 * vectors; since we may hit more than one enemy and we want messages for all.
 */

		std::vector<std::shared_ptr<Entity>> entities_hit;
		std::vector<int> total_damage;
		std::vector<Ranged_hit_type> best_hit;
/* We track all the tiles_hit by all pellets.  After we finish processing all
 * pellets, we move (target) to the average of all pellets.
 */
		std::vector<Tripoint> tiles_hit;

		for (int pellet = 0; pellet < num_pellets; pellet++)
		{
			int angle_missed_by = attack.roll_variance();
/*
      if (TESTING_MODE) {
        debugmsg("angle %d (%s)", angle_missed_by, attack.variance.str().c_str());
      }
*/
// Use 1800 since attack.variance is measured in 10ths of a degree
			double distance_missed_by = range * tan(angle_missed_by * PI / 1800);
			int tiles_off = int(distance_missed_by);
			Tripoint curtarget = target;
			if (tiles_off >= 1)
			{
				curtarget.x += rng(0 - tiles_off, tiles_off);
				curtarget.y += rng(0 - tiles_off, tiles_off);
			}
			tiles_hit.push_back(curtarget);
// fine_distance is used later to see if we hit the target or "barely missed"
			int fine_distance = 100 * (distance_missed_by - tiles_off);

			std::vector<Tripoint> path = map->line_of_sight(origin, curtarget);
			if (path.empty())
			{ // Lost line of sight at some point
				path = line_to(origin, curtarget);
			}

// We track i outside of the function, because we need it to know where the
// projectile stopped.
			int i = 0;
			bool stopped = false;
			while (!stopped && i < path.size())
			{
				if (map->move_cost(path[i].x, path[i].y) == 0)
				{
// It's a solid tile, so let's try to smash through it!
					map->smash(path[i].x, path[i].y, attack.roll_damage(), false);
					if (map->move_cost(path[i].x, path[i].y) == 0)
					{
						stopped = true; // Couldn't get through the terrain!
						i--; // Stop at the terrain before the solid one
					}
				}
				else
				{
// Drop a field in our wake?
					if (attack.wake_field.exists())
					{
						attack.wake_field.drop(Tripoint(path[i].x, path[i].y, path[i].z),
								shooter_name);
					}
// Did we hit an entity?
					std::shared_ptr<Entity> entity_hit = entities.entity_at(path[i].x, path[i].y);

					if (entity_hit)
					{
						bool hit;
						Ranged_hit_type hit_type;
						if (i == path.size() - 1)
						{
							hit = rng(0, 100) >= fine_distance;
							if (fine_distance <= 10)
							{
								hit_type = RANGED_HIT_HEADSHOT;
							}
							else if (fine_distance <= 35)
							{
								hit_type = RANGED_HIT_CRITICAL;
							}
							else if (fine_distance <= 75)
							{
								hit_type = RANGED_HIT_NORMAL;
							}
							else
							{
								hit_type = RANGED_HIT_GRAZE;
							}
						}
						else
						{
							hit = one_in(3);// TODO: Incorporate the size of the monster
							if (hit)
							{
								int hit_roll = rng(1, 100);
								if (hit_roll <= 10)
								{
									hit_type = RANGED_HIT_HEADSHOT;
								}
								else if (hit_roll <= 35)
								{
									hit_type = RANGED_HIT_CRITICAL;
								}
								else if (hit_roll <= 75)
								{
									hit_type = RANGED_HIT_NORMAL;
								}
								else
								{
									hit_type = RANGED_HIT_GRAZE;
								}
							}
						}

						if (hit)
						{
// Check if we've already hit this entity once before.
							bool found_entity = false;
							int entity_index;
							for (entity_index = 0;
								 !found_entity && entity_index < entities_hit.size();
								 entity_index++)
							{
								if (entities_hit[entity_index].get() == entity_hit.get())
								{
									found_entity = true;
								}
							}
							if (!found_entity)
							{
								entities_hit.push_back(entity_hit);
								total_damage.push_back(0);
								best_hit.push_back(RANGED_HIT_NULL);
								entity_index = entities_hit.size() - 1;
							}

							Damage_set dam = attack.roll_damage(hit_type);

							total_damage[entity_index] += dam.get_damage(DAMAGE_PIERCE);
							if (hit_type > best_hit[entity_index])
							{
								best_hit[entity_index] = hit_type;
							}
// Outstanding bug!  Sometimes ranged attacks inexplicably do 0 damage.  I'm
// leaving this in to help track it down, maybe.
//							if (dam.get_damage(DAMAGE_PIERCE) == 0 && TESTING_MODE)
//							{
//								debugmsg("0 ranged damage!");
//								debugmsg("Attack damage: %d %d %d",
//										attack.damage[DAMAGE_BASH],
//										attack.damage[DAMAGE_CUT],
//										attack.damage[DAMAGE_PIERCE]);
//							}
							stopped = true;
						}
						else if (i == path.size() - 1 && shooter == player.get())
						{
							messageQueue.addMessage({ Doryen::format("<c=dkgray>{} barely {} {}.<c=/>",
									shooter_name, miss_verb, entity_hit->get_name_to_player()) });
						}
					} // if (entity hit)
				} // End of <Didn't hit solid terrain>
				i++;  // Increment which tile in the trajectory we're examining
			} // while (!stopped && i < path.size())

			Tripoint end_point;
			if (i == path.size())
			{
				end_point = Tripoint(path.back().x, path.back().y, 0);
			}
			else
			{
				end_point = Tripoint(path[i].x, path[i].y, 0);
			}
// Drop the projectile we threw, if it's "real" and we're throwing it
			if (it.is_real() && attack.type == RANGED_ATT_THROW)
			{
				map->add_item(it, end_point);
			}
// Create the target_field from our attack, if it's "real"
			if (attack.target_field.exists())
			{
				attack.target_field.drop(end_point, shooter_name);
			}
// Now, display messages for all entities hit.
// Sanity check for vector sizes!
			if (entities_hit.size() != total_damage.size() ||
				entities_hit.size() != best_hit.size())
			{
//				debugmsg("entities_hit %d, total_damage %d, best_hit %d",
//						entities_hit.size(), total_damage.size(), best_hit.size());
			}
			else
			{
				for (int i = 0; i < entities_hit.size(); i++)
				{
					std::shared_ptr<Entity> entity_hit = entities_hit[i];
					Ranged_hit_type hit_type = best_hit[i];
					int dam = total_damage[i];
					if (hit_type == RANGED_HIT_HEADSHOT)
					{
						shooter_name = capitalize(shooter_name);
						messageQueue.addMessage({ Doryen::format("<c=ltred>Headshot!  {} {} {} for {d} damage!<c=/>",
								shooter_name, verb, entity_hit->get_name_to_player(),dam) });
					}
					else if (hit_type == RANGED_HIT_CRITICAL)
					{
						shooter_name = capitalize(shooter_name);
						messageQueue.addMessage({ Doryen::format("<c=ltred>Critical!  {} {} {} for {d} damage!<c=/>",
								shooter_name, verb, entity_hit->get_name_to_player(), dam) });
					}
					else if (hit_type == RANGED_HIT_GRAZE)
					{
						messageQueue.addMessage({ Doryen::format("<c=ltred>{} {} {} for {d} damage!<c=/>",
								shooter_name, graze_verb, entity_hit->get_name_to_player(),dam) });
					}
					else
					{
						messageQueue.addMessage({ Doryen::format("<c=ltred>{} {} {} for {d} damage!<c=/>",
								shooter_name, verb, entity_hit->get_name_to_player(),dam) });
					}
// Figure out the part hit, in case it's the player or NPC
					Body_part part_hit;
					if (hit_type == RANGED_HIT_HEADSHOT)
					{
						part_hit = random_head_part();
					}
					else if (hit_type == RANGED_HIT_CRITICAL)
					{
						part_hit = BODY_PART_TORSO;
					}
					else if (hit_type == RANGED_HIT_GRAZE || one_in(3))
					{
						part_hit = random_extremity();
					}
					else
					{
						part_hit = BODY_PART_TORSO;
					}

					entity_hit->take_damage(DAMAGE_PIERCE, dam, shooter_name, part_hit);
				} //for (int i = 0; i < entities_hit.size(); i++)
			} // Sanity check passed
		} // for (int pellet = 0; pellet < attack.pellets; pellet++)
// Now we have to move target to the average of all the tiles hit by pellets
		int new_x = 0, new_y = 0;
		for (int i = 0; i < tiles_hit.size(); i++)
		{
			new_x += tiles_hit[i].x;
			new_y += tiles_hit[i].y;
		}
// We want to round randomly - not always round down.
		int remainder_x = new_x % tiles_hit.size(),
				remainder_y = new_y % tiles_hit.size();
		new_x /= tiles_hit.size();
		new_y /= tiles_hit.size();
		if (rng(1, tiles_hit.size()) <= remainder_x)
		{
			new_x--;
		}
		if (rng(1, tiles_hit.size()) <= remainder_y)
		{
			new_y--;
		}
/* Finally, if we were originally targeting an entity, and we are no longer
 * pointed at an entity, we may have a chance to retarget.
 */
		if (targeting_entity && retarget_range > 0 &&
			entities.entity_at(new_x, new_y, target.z))
		{
			std::vector<Tripoint> new_targets;
			for (int ntx = new_x - retarget_range; ntx <= new_x + retarget_range;
				 ntx++)
			{
				for (int nty = new_y - retarget_range; nty <= new_y + retarget_range;
					 nty++)
				{
					std::shared_ptr<Entity> new_target = entities.entity_at(ntx, nty, target.z);
// TODO: Ensure that the new target isn't friendly!
					if (new_target)
					{
						new_targets.push_back(Tripoint(ntx, nty, target.z));
					}
				}
			}
			if (!new_targets.empty())
			{
				int nt_index = rng(0, new_targets.size() - 1);
				new_x = new_targets[nt_index].x;
				new_y = new_targets[nt_index].y;
			}
		}
		target.x = new_x;
		target.y = new_y;

	} // for (int round = 0; round < attack.rounds; round++)
}

void Projectile::launch(Entity* shooter, Ranged_attack attack, Tripoint origin, Tripoint target)
{
	launch(shooter, Item(), attack, origin, target);
}

void Projectile::launch(Ranged_attack attack, Tripoint origin, Tripoint target)
{
	projectile.launch(NULL, attack, origin, target);
}
