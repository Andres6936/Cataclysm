#include <Cataclysm/Util/files.h>
#include <Cataclysm/Util/globals.h>
#include <Cataclysm/Util/String/stringfunc.h>
#include <Cataclysm/Random/rng.h>
#include <Cataclysm/Screen/PlayScreen.hpp>
#include <Cataclysm/Mechanism/Projectile.hpp>
#include <Cataclysm/Entity/Monster/monster.h>
#include <Cataclysm/Mechanism/TimeManager.hpp>
#include <Cataclysm/Item/ActiveItemsManager.hpp>
#include <Cataclysm/Mechanism/NextItemManager.hpp>
#include <Cataclysm/Visual/Screen/MessageQueue.hpp>

#include <stdarg.h>

std::vector<std::string> get_pickup_strings(std::vector<Item>* items,
		std::vector<bool>* picking_up);

std::string pickup_string(Item* item, char letter, bool picking_up);

PlayScreen::PlayScreen()
{
	player = NULL;
	last_target = -1;
	new_messages = 0;
	next_furniture_uid = 0;
	game_over = false;
}

void PlayScreen::reset_temp_values()
{
	timeManager.setLightLevel(0);
}

Cataclysm::ScreenType PlayScreen::do_action(Interface_action act)
{
	switch (act)
	{
	case IACTION_MOVE_N:
		player_move(0, -1);
		break;
	case IACTION_MOVE_S:
		player_move(0, 1);
		break;
	case IACTION_MOVE_W:
		player_move(-1, 0);
		break;
	case IACTION_MOVE_E:
		player_move(1, 0);
		break;
	case IACTION_MOVE_NW:
		player_move(-1, -1);
		break;
	case IACTION_MOVE_NE:
		player_move(1, -1);
		break;
	case IACTION_MOVE_SW:
		player_move(-1, 1);
		break;
	case IACTION_MOVE_SE:
		player_move(1, 1);
		break;
	case IACTION_PAUSE:
		player->pause();
		break;

	case IACTION_MOVE_UP:
		if (!map->has_flag(TF_STAIRS_UP, player->pos))
		{
			messageQueue.addMessage({ "You cannot go up here." });
			player_move_vertical(1);  // Snuck this in for debugging purposes
		}
		else
		{
			player_move_vertical(1);
		}
		break;

	case IACTION_MOVE_DOWN:
		if (!map->has_flag(TF_STAIRS_DOWN, player->pos))
		{
			messageQueue.addMessage({ "You cannot go down here." });
			player_move_vertical(-1); // Snuck this in for debugging purposes
		}
		else
		{
			player_move_vertical(-1);
		}
		break;

	case IACTION_PICK_UP:
// Make sure we're not sealed...
		if (map->has_flag(TF_SEALED, player->pos))
		{
			messageQueue.addMessage({ Doryen::format("<c=dkgray>That {} is sealed; you cannot retrieve items there.<c=/>",
									  map->get_name(player->pos)) });

		}
		else if (map->item_count(player->pos) == 0)
		{
			messageQueue.addMessage({ "No items here." });

		}
		else if (map->item_count(player->pos) == 1)
		{
// Only one item - no need for the interface
			std::vector<Item>* items = map->items_at(player->pos);
			if (player->add_item((*items)[0]))
			{
				items->clear();
			}
		}
		else
		{
			pickup_items(player->pos);
		}
		break;

	case IACTION_OPEN:
	{
		messageQueue.addMessage({ "<c=ltgreen>Open where? (Press direction key)<c=/>" });
		draw_all();
		Point dir = input_direction(input());
		if (dir.x == -2)
		{ // Error
			messageQueue.addMessage({ "Invalid direction." });
		}
		else
		{
			Tripoint open = player->pos + dir;
			if (map->apply_signal("open", open, player.get()))
			{
				player->use_ap(100);
			}
		}
	}
		break;

	case IACTION_CLOSE:
	{
		messageQueue.addMessage({ "<c=ltgreen>Close where? (Press direction key)<c=/>" });
		draw_all();
		Point dir = input_direction(input());
		if (dir.x == -2)
		{ // Error
			messageQueue.addMessage({ "Invalid direction." });
		}
		else
		{
			Tripoint close = player->pos + dir;
			Entity* ent = entities.entity_at(close);
			if (ent == player.get())
			{
				messageQueue.addMessage({ "Maybe you should move out of the doorway first." });
			}
			else if (ent)
			{
				messageQueue.addMessage({ Doryen::format("There's a {} in the way.", ent->get_name()) });
			}
			else if (map->furniture_at(close))
			{
				messageQueue.addMessage({ "There's some furniture in the way." });
			}
			else if (map->apply_signal("close", close, player.get()))
			{
				player->use_ap(100);
			}
		}
	}
		break;

	case IACTION_SMASH:
	{
		messageQueue.addMessage({ "<c=ltgreen>Smash where? (Press direction key)<c=/>" });
		draw_all();
		Point dir = input_direction(input());
		if (dir.x == -2)
		{ // Error
			messageQueue.addMessage({ "Invalid direction." });
		}
		else
		{
			Tripoint sm = player->pos + dir;
			if (!map->is_smashable(sm))
			{
				messageQueue.addMessage({ "Nothing to smash there." });
			}
			else
			{
				messageQueue.addMessage({ Doryen::format("You smash the {}.", map->get_name(sm)) });
				map->smash(sm, player->std_attack().roll_damage());
				player->use_ap(100);
			}
		}
	}
		break;

	case IACTION_EXAMINE:
	{
		messageQueue.addMessage({ "<c=ltgreen>Examine where? (Press direction key)<c=/>" });
		draw_all();
		Point dir = input_direction(input());
		if (dir.x == -2)
		{ // Error
			messageQueue.addMessage({ "Invalid direction." });
		}
		else
		{
			Tripoint examine = player->pos + dir;
// Can't pick up items if we're sealed
			if (map->has_flag(TF_SEALED, examine))
			{
				messageQueue.addMessage({ Doryen::format("<c=dkgray>That {} is sealed; you cannot retrieve items there.<c=/>",
										  map->get_name(examine)) });

			}
			else if (map->item_count(examine) > 0)
			{
				pickup_items(examine);
			}

			std::stringstream description;
			description << "That is " << map->get_name_indefinite(examine) << ".";
			if (map->furniture_at(examine))
			{
				description << "  You can drag it using the <c=yellow>grab<c=/> " <<
							"command (<c=yellow>" <<
							KEYBINDINGS.describe_bindings_for(IACTION_GRAB) <<
							"<c=/>).";
				if (TESTING_MODE)
				{
					messageQueue.addMessage({ Doryen::format("<c=pink>Furniture uid {d}.<c=/>", map->furniture_at(examine)->get_uid()) });
				}
			}

			messageQueue.addMessage({ description.str() });
		}
	}
		break;

	case IACTION_GRAB:
	{
		if (player->dragged.empty())
		{
			messageQueue.addMessage({ "<c=ltgreen>Grab where? (Press direction key)<c=/>" });
			draw_all();
			Point dir = input_direction(input());
			if (dir.x == -2)
			{ // Error
				messageQueue.addMessage({ "Invalid direction." });
			}
			else
			{
				Tripoint target = player->pos + dir;
				player->dragged = map->grab_furniture(player->pos, target);
				if (player->dragged.empty())
				{
					messageQueue.addMessage({ "Nothing to grab there." });
				}
				else
				{
					messageQueue.addMessage({ Doryen::format("You grab the {}.", player->get_dragged_name()) });
					if (TESTING_MODE)
					{
						for (int i = 0; i < player->dragged.size(); i++)
						{
							messageQueue.addMessage({ player->dragged[i].pos.str() });
						}
					}
				}
			}
		}
		else
		{  // We're already dragging something; so let go!
			messageQueue.addMessage({ Doryen::format("You let go of the {}.", player->get_dragged_name()) });
			player->dragged.clear();
		}
	}
		break;

	case IACTION_INVENTORY:
		return Cataclysm::ScreenType::INVENTORY_SINGLE_SELECTION;

	case IACTION_DROP:
	{
		std::vector<Item> dropped = player->drop_items();
		std::stringstream message;
		message << "You drop " << list_items(&dropped);
		for (int i = 0; i < dropped.size(); i++)
		{
			map->add_item(dropped[i], player->pos);
		}
		messageQueue.addMessage({ message.str() });
	}
		break;

	case IACTION_WIELD:
	{
		Item it = player->inventory_single();
		messageQueue.addMessage({ player->sheath_weapon_message() });
		player->sheath_weapon();
		messageQueue.addMessage({ player->wield_item_message(it) });
		player->wield_item_uid(it.get_uid());
	}
		break;

	case IACTION_WEAR:
	{
		Item it = player->inventory_single();
		messageQueue.addMessage({ player->wear_item_message(it) });
		player->wear_item_uid(it.get_uid());
	}
		break;

	case IACTION_TAKE_OFF:
	{
		Item it = player->inventory_single();
		messageQueue.addMessage({ player->take_off_item_message(it) });
		player->take_off_item_uid(it.get_uid());
	}
		break;

	case IACTION_APPLY:
	{
		Item it = player->inventory_single();
		Item_action act = it.default_action();

		switch (act)
		{
		case IACT_NULL:
			messageQueue.addMessage({ "Can't do anything with that item." });
			break;

		case IACT_WIELD:
			messageQueue.addMessage({ player->wield_item_message(it) });
			player->wield_item_uid(it.get_uid());
			break;

		case IACT_WEAR:
			messageQueue.addMessage({ player->wear_item_message(it) });
			player->wear_item_uid(it.get_uid());
			break;

		case IACT_EAT:
			messageQueue.addMessage({ player->eat_item_message(it) });
			player->eat_item_uid(it.get_uid());
			break;

		case IACT_APPLY:
// Need to redraw the map
			messageQueue.addMessage({ player->apply_item_message(it) });
			player->apply_item_uid(it.get_uid());
			break;

		case IACT_READ:
			messageQueue.addMessage({ player->read_item_message(it) });
			player->read_item_uid(it.get_uid());
			break;
		}
	}
		break;

	case IACTION_READ:
	{
		Item it = player->inventory_single();
		messageQueue.addMessage({ player->read_item_message(it) });
		player->read_item_uid(it.get_uid());
	}
		break;


	case IACTION_RELOAD:
	{
		Item it = player->inventory_single();
		player->reload_prep(it.get_uid());
	}
		break;

	case IACTION_RELOAD_EQUIPPED:
		player->reload_prep(player->weapon.get_uid());
		break;

	case IACTION_THROW:
	{
		Item it = player->inventory_single();
		if (!it.is_real())
		{
			messageQueue.addMessage({ "Never mind." });
		}
		else
		{
			int x = player->pos.x, y = player->pos.y,
					range = player->weapon.get_fired_attack().range;
			Tripoint target = target_selector(x, y, range, true, true);
			if (target.x == -1)
			{ // We canceled
				messageQueue.addMessage({ "Never mind." });
			}
			else
			{
// If we actually targeted an entity, set that to our last target.
				Entity* targeted_entity = entities.entity_at(target);
				if (targeted_entity)
				{
					last_target = targeted_entity->uid;
				}
				player->remove_item_uid(it.get_uid(), 1);
				Ranged_attack att = player->throw_item(it);

				projectile.launch(player.get(), it, att, player->pos, target);
			}
		}
	}
		break;

	case IACTION_FIRE:
		if (player->can_fire_weapon())
		{
			int x = player->pos.x, y = player->pos.y,
					range = player->weapon.get_fired_attack().range;
			Tripoint target = target_selector(x, y, range, true, true);
			if (target.x == -1)
			{ // We canceled
				messageQueue.addMessage({ "Never mind." });
// If we target ourself, confirm that we want to shoot ourselves...
			}
			else if (target != player->pos ||
					 msg_query_yn("Really target yourself?"))
			{
// If we actually targeted an entity, set that to our last target.
				Entity* targeted_entity = entities.entity_at(target);
				if (targeted_entity)
				{
					last_target = targeted_entity->uid;
				}
// And do the actual attack!
				Ranged_attack att = player->fire_weapon();

				projectile.launch(player.get(), player->weapon, att, player->pos, target);
			}
		}
		break;

	case IACTION_ADVANCE_FIRE_MODE:
		player->weapon.advance_fire_mode();
		messageQueue.addMessage({ player->advance_fire_mode_message() });
		break;

	case IACTION_EAT:
	{
		Item it = player->inventory_single();
		messageQueue.addMessage({ player->eat_item_message(it) });
		player->eat_item_uid(it.get_uid());
	}
		break;

	case IACTION_MESSAGES_SCROLL_BACK:
		i_hud.add_data("text_messages", -1);
		break;

	case IACTION_MESSAGES_SCROLL_FORWARD:
		i_hud.add_data("text_messages", 1);
		break;

	case IACTION_VIEW_WORLDMAP:
	{
		Point p = map->get_center_point();
		Point got = worldmap->get_point(p.x, p.y);
// Adjust to match the upper-left corner
		got.x -= MAP_SIZE / 2;
		got.y -= MAP_SIZE / 2;
		if (TESTING_MODE)
		{
			int posx = got.x, posy = got.y;
			posx += MAP_SIZE / 2;
			posy += MAP_SIZE / 2;
			SUBMAP_POOL.load_area_centered_on(posx, posy);
			map->generate(worldmap.get(), got.x, got.y);
			Point mp = map->get_center_point();
			//debugmsg("Worldmap %s, map %s", got.str().c_str(), mp.str().c_str());
		}
	}
		break;

	case IACTION_CHAR_STATUS:
		player->status_interface();
		break;

	case IACTION_CHAR_SKILLS:
		return Cataclysm::ScreenType::SKILL;

	case IACTION_DEBUG:
		if (!TESTING_MODE)
		{
			messageQueue.addMessage({ "<c=red>To access debug commands, run the program with the --test flag." });
		}
		else
		{
			debug_command();
		}
		break;

	case IACTION_QUIT:
		if (showQueryYesNo("Commit suicide?"))
		{
			game_over = true;
			player->action_points = 0;
		}

		return Cataclysm::ScreenType::QUIT;
	}

	return Cataclysm::ScreenType::NONE;
}

void PlayScreen::move_entities()
{
	clean_up_dead_entities();
	//scent_map = map->get_dijkstra_map(player->pos, 15);
// First, give all entities action points
	for (std::list<Entity*>::iterator it = entities.instances.begin();
		 it != entities.instances.end();
		 it++)
	{
		if (!(*it)->is_player())
		{
			(*it)->gain_action_points();
		}
	}
/* Loop through the entities, giving each one a single turn at a time.
 * Stop when we go through a loop without finding any entities that can
 * take a turn.
 */
	bool all_done = true;
	do
	{
		all_done = true;
		for (std::list<Entity*>::iterator it = entities.instances.begin();
			 it != entities.instances.end();
			 it++)
		{
			Entity* ent = *it;
			if (!ent->is_player() && ent->action_points > 0 && !ent->dead)
			{
				ent->take_turn();
				all_done = false;
			}
		}
	} while (!all_done);

	clean_up_dead_entities(); // Just in case an entity killed itself

}

void PlayScreen::clean_up_dead_entities()
{
	std::list<Entity*>::iterator it = entities.instances.begin();
	while (it != entities.instances.end())
	{
		Entity* ent = (*it);
		if (ent->dead)
		{
			if (player->can_see(map.get(), ent->pos))
			{
				if (ent->killed_by_player)
				{
					messageQueue.addMessage({ Doryen::format("You kill {}!", ent->get_name_to_player()) });
				}
				else
				{
					messageQueue.addMessage({ Doryen::format("{} dies!", ent->get_name_to_player()) });
				}
			}
			ent->die();
			delete ent;
			it = entities.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void PlayScreen::handle_player_activity()
{
	if (!player->has_activity())
	{
		return;
	}
	Player_activity* act = &(player->activity);
	if (act->duration <= player->action_points)
	{
		player->action_points -= act->duration;
		complete_player_activity();
	}
	else
	{
		act->duration -= player->action_points;
		player->action_points = 0;
	}
}

void PlayScreen::complete_player_activity()
{
	Player_activity* act = &(player->activity);
	switch (act->type)
	{

	case PLAYER_ACTIVITY_NULL:
	case PLAYER_ACTIVITY_WAIT:
		break; // Nothing happens

	case PLAYER_ACTIVITY_RELOAD:
	{
		Item* reloaded = player->ref_item_uid(act->primary_item_uid);
		if (!reloaded)
		{
			debugmsg("Completed reload, but the item wasn't there!");
			return;
		}
		messageQueue.addMessage({ Doryen::format("You reload your {}.", reloaded->get_name()) });
		reloaded->reload(player.get(), act->secondary_item_uid);
	}
		break;

	case PLAYER_ACTIVITY_READ:
	{
		Item* read = player->ref_item_uid(act->primary_item_uid);
		if (!read)
		{
			debugmsg("Completed reading, but the item wasn't there!");
			return;
		}
		messageQueue.addMessage({ "You finish reading." });
		player->finish_reading(read);
	}
		break;

	default:
		debugmsg("Our switch doesn't know how to handle action %d, '%s'",
				act->type, act->get_name().c_str());
	}
	player->activity = Player_activity();
}

void PlayScreen::process_active_items()
{
	for (auto& item : activeItemsManager)
	{
		item->process_active();
	}
}

void PlayScreen::shift_if_needed()
{
	//return;
	int min = SUBMAP_SIZE * (MAP_SIZE / 2), max = min + SUBMAP_SIZE - 1;
	int shiftx = 0, shifty = 0;
	if (player->pos.x < min)
	{
		shiftx = -1 + (player->pos.x - min) / SUBMAP_SIZE;
	}
	else if (player->pos.x > max)
	{
		shiftx = 1 + (player->pos.x - max) / SUBMAP_SIZE;
	}
	if (player->pos.y < min)
	{
		shifty = -1 + (player->pos.y - min) / SUBMAP_SIZE;
	}
	else if (player->pos.y > max)
	{
		shifty = 1 + (player->pos.y - max) / SUBMAP_SIZE;
	}
	int posx = map->posx + shiftx, posy = map->posy + shifty;
	posx += MAP_SIZE / 2;
	posy += MAP_SIZE / 2;
	SUBMAP_POOL.load_area_centered_on(posx, posy);
	map->shift(worldmap.get(), shiftx, shifty);
	//Point p = map->get_center_point();
	//SUBMAP_POOL.load_area_centered_on( p.x, p.y );
	for (std::list<Entity*>::iterator it = entities.instances.begin();
		 it != entities.instances.end();
		 it++)
	{
		(*it)->shift(shiftx, shifty);
	}
}

void PlayScreen::player_move(int xdif, int ydif)
{
// TODO: Remove this?
	if (xdif < -1 || xdif > 1 || ydif < -1 || ydif > 1)
	{
		debugmsg("Game::player_move called with [%d, %d]", xdif, ydif);
		return;
	}

	int newx = player->pos.x + xdif, newy = player->pos.y + ydif;
	Entity* ent = entities.entity_at(newx, newy, player->pos.z);
	std::string tername = map->get_name(newx, newy, player->pos.z);

// If we bump an entity, attack it.
// TODO: If that entity is a friendly NPC, talk instead!
	if (ent)
	{
		player->attack(ent);

// Check that we can drag our furniture (if any)
	}
	else if (!player->can_drag_furniture_to(map.get(), newx, newy))
	{
		messageQueue.addMessage({ Doryen::format("The {} you're dragging prevents you from moving there.", player->get_dragged_name()) });
		messageQueue.addMessage({ Doryen::format("Press (<c=yellow>{}<c=/>) to drop it.", KEYBINDINGS.describe_bindings_for(IACTION_GRAB)) });
		return;

// If it's open space, we'll fall!
	}
	else if (map->has_flag(TF_OPEN_SPACE, newx, newy))
	{
		int levels_to_fall = 0;
		int fall_z = player->pos.z;
		while (map->has_flag(TF_OPEN_SPACE, newx, newy, fall_z))
		{
			levels_to_fall++;
			fall_z--;
		}
		if (!msg_query_yn("Step out into open space and fall %d floors?",
				levels_to_fall))
		{
			return;
		}
		if (TESTING_MODE)
		{
			debugmsg("Fall");
		}
		player->move_to(map.get(), newx, newy);
		player_move_vertical(0 - levels_to_fall);
		player->fall(levels_to_fall); // This handles damage, etc.

// When we fall hit land hard; getting ready for our next turn takes time
		int move_penalty = (120 - 2 * player->stats.dexterity);
// Low dexterity means we're really bad at recovering from a fall
		if (player->stats.dexterity < 10)
		{
			move_penalty += 5 * (10 - player->stats.dexterity);
		}
		move_penalty -= 3 * player->skills.get_level(SKILL_DODGE);
		player->use_ap(100 + levels_to_fall * move_penalty);

// If we can move there... move there!
	}
	else if (player->can_move_to(map.get(), newx, newy))
	{
		player->move_to(map.get(), newx, newy);

// Otherwise, try to open it?
	}
	else if (map->apply_signal("open", newx, newy, player->pos.z, player.get()))
	{
		player->use_ap(100);
		return; // Don't list items
	}
// List items here, unless it's sealed.
	if (map->has_flag(TF_SEALED, player->pos))
	{
		messageQueue.addMessage(
				{ Doryen::format("<c=dkgray>This {} is sealed; there may be items inside, but you cannot see or access them.<c=/>", map->get_name(player->pos)) });
	}
	else
	{
		std::vector<Item>* items = map->items_at(player->pos);
// TODO: Ensure the player has the sense of sight
		if (!items->empty())
		{
			std::string item_message = "You see here " + list_items(items);
			messageQueue.addMessage({ item_message });
		}
	}
}

void PlayScreen::player_move_vertical(int zdif)
{
// FRODO: Move entities into a stairs-following queue
//        (except not, since we're on a 3D map nowadays)
	map->shift(worldmap.get(), 0, 0, zdif);
	player->pos.z += zdif;
}

bool PlayScreen::msg_query_yn(std::string msg, ...)
{
// This duplicates all the code of add_msg(), but there's no other option!
	if (msg.empty())
	{
		return true;
	}
	char buff[2048];
	va_list ap;
	va_start(ap, msg);
	vsprintf(buff, msg.c_str(), ap);
	va_end(ap);
	std::string text(buff);
	text = capitalize(text);
// Add the color highlighting that lets the player know it's a prompt
	std::stringstream colorized;
	colorized << "<c=ltgreen>" << text << "<c=/>";
	text = colorized.str();

	messageQueue.addMessage({text, static_cast<uint32_t>(timeManager.getTime().get_turn())});
	new_messages += 1;

	draw_all();
	long ch = input();
	if (ch != 'Y' && ch != 'N')
	{
		messageQueue.addMessage({ "<c=ltred>Y<c=ltgreen> or <c=ltred>N<c=ltgreen> only, please. Case-sensitive." });
	}
	while (ch != 'Y' && ch != 'N')
	{
		ch = input();
	}
	return (ch == 'Y');
}

void PlayScreen::remove_active_item_uid(int uid)
{
	for (int i = 0; i < activeItemsManager.size(); i++)
	{
		if (activeItemsManager[i]->get_uid() == uid)
		{
			activeItemsManager.erase(activeItemsManager.begin() + i);
			return;
		}
	}
}

// Bit of a duplication of code from find_item(), but what can ya do
bool PlayScreen::destroy_item(Item* it, int uid)
{
	// Sanity check
	if (it == NULL && (uid < 0 || uid >= nextItemManager.getNextItemUid()))
	{
		return false;
	}

	// Check entities first - almost certainly faster than the map
	for (std::list<Entity*>::iterator iter = entities.instances.begin();
		 iter != entities.instances.end();
		 iter++)
	{
		Item check = (*iter)->remove_item(it, uid);
		if (check.is_real())
		{
			return true;
		}
	}
	return map->remove_item(it, uid);
}

bool PlayScreen::destroy_item_uid(int uid)
{
	return destroy_item(NULL, uid);
}

void PlayScreen::draw_all()
{
	update_hud();
	int range = player->sight_range(get_light_level());

	map->draw(w_map, &entities, player->pos, range);
	map->draw(screenMap, &entities, player->pos, range);

	w_map->refresh();

	screenMap.blit({0, 0}, console, {0, 0});
	screenHUD.blit({0, 0}, console, {24, 0});

	console.draw();
}

void PlayScreen::update_hud()
{
//debugmsg("update_hud(); mes %d, new %d", messages.size(), new_messages);
	print_messages();
// Update date
	i_hud.set_data("text_date", timeManager.getTime().get_text());
// Update location description
	Submap* sm = map->get_center_submap();
// TODO: This prints the DATA name of the World_ter; we want the DISPLAY name
	if (sm)
	{
		i_hud.set_data("text_location", sm->get_world_ter_name());
	}
// Update speed
	i_hud.set_data("num_speed", player->get_speed());
// Colorize speed
	i_hud.set_data("num_speed", player->get_speed_color());
// Add any player ailments
	std::string status = player->get_all_status_text();
	//std::string status = player->get_hunger_text() + " " + player->get_thirst_text();
	i_hud.set_data("text_status", status);
// Draw minimap
	cuss::element* minimap = i_hud.find_by_name("draw_minimap");
	if (minimap)
	{
		int cornerx = map->posx - minimap->sizex / 2 + MAP_SIZE / 2;
		int cornery = map->posy - minimap->sizey / 2 + MAP_SIZE / 2;
		worldmap->draw_minimap(minimap, cornerx, cornery);
	}
	if (player->weapon.is_real())
	{
		std::stringstream weapon_ss;
		weapon_ss << "<c=white>" << player->weapon.get_name_full() << "<c=/>";
		i_hud.set_data("text_weapon", weapon_ss.str());
	}
	else
	{
		i_hud.set_data("text_weapon", "None");
	}
	i_hud.set_data("hp_head", player->hp_text(HP_PART_HEAD));
	i_hud.set_data("hp_torso", player->hp_text(HP_PART_TORSO));
	i_hud.set_data("hp_l_arm", player->hp_text(HP_PART_LEFT_ARM));
	i_hud.set_data("hp_r_arm", player->hp_text(HP_PART_RIGHT_ARM));
	i_hud.set_data("hp_l_leg", player->hp_text(HP_PART_LEFT_LEG));
	i_hud.set_data("hp_r_leg", player->hp_text(HP_PART_RIGHT_LEG));

//debugmsg("Offset %d\n%s", i_hud.get_int("text_messages"), i_hud.get_str("text_messages").c_str());
	i_hud.draw(w_hud);
	i_hud.draw(screenHUD);

	w_hud->refresh();
}

void PlayScreen::print_messages()
{
	i_hud.clear_data("text_messages");

	cuss::element* message_box = i_hud.find_by_name("text_messages");

	if (!message_box)
	{
		debugmsg("Couldn't find text_messages in i_hud!");
		return;
	}

	std::uint32_t start = messageQueue.getSize() - message_box->sizey;

	if (start < 0)
	{
		start = 0;
	}

	for (unsigned int i = start; i < messageQueue.getSize(); i++)
	{
		try
		{
			i_hud.add_data("text_messages", messageQueue.getMessageAt(i));
		}
		catch (std::out_of_range& exception)
		{
			showDebugMessage(exception.what());
		}
	}
}

void PlayScreen::debug_command()
{
	messageQueue.addMessage({ "<c=yellow>Press debug key.<c=/>" });
	draw_all();
	long ch = input();
	Debug_action act = KEYBINDINGS.bound_to_debug_key(ch);

	switch (act)
	{
	case DEBUG_ACTION_NULL:
		messageQueue.addMessage({ "<c=dkgray>Never mind.<c=/>" });
		break;

	case DEBUG_ACTION_CREATE_ITEM:
	{
		std::string name = string_input_popup("Item name (partial names OK):");
		Item_type* type = ITEM_TYPES.lookup_partial_name(name);
		if (!type)
		{
			messageQueue.addMessage({ Doryen::format("<c=dkgray>'{}' did not match any items.<c=/>", name) });
		}
		else
		{
			Item spawned(type);
			map->add_item(spawned, player->pos);
			messageQueue.addMessage(
					{ Doryen::format("Spawned {} (UID {d}, next UID {}).", spawned.get_name_indefinite(), spawned.get_uid(),
					  nextItemManager.getNextItemUid()) });
		}
	}
		break;

	case DEBUG_ACTION_BUILD_MAP:
	{
		std::string name = string_input_popup("Map layout name:");
		Mapgen_spec* spec = MAPGEN_SPECS.lookup_name(name);
		if (spec)
		{
			Submap* sm = map->get_testing_submap();
			if (sm)
			{
				spec->prepare();
				sm->clear_items();
				sm->generate(spec);
			}
			else
			{
				messageQueue.addMessage({ "Map::get_testing_submap() return NULL???" });
			}
		}
		else
		{
			messageQueue.addMessage({ Doryen::format("<c=dkgray>Layout '{}}' does not exist.", name) });
		}
	}
		break;

	case DEBUG_ACTION_MAP_INFO:
		messageQueue.addMessage({ map->get_center_submap()->get_spec_name() });
		break;

	case DEBUG_ACTION_PLACE_FIELD:
	{
		std::string name = string_input_popup("Field type (partial name OK):");
		Field_type* type = FIELDS.lookup_partial_name(name);
		if (!type)
		{
			messageQueue.addMessage({ Doryen::format("<c=dkgray>'{}' did not match any fields.<c=/>", name) });
		}
		else
		{
			Tripoint pos = target_selector();
			map->add_field(type, pos, "Magic");
		}
	}
		break;

	case DEBUG_ACTION_CLEAR_ITEMS:
		map->clear_items();
		messageQueue.addMessage({ "Items cleared.  Note; this may cause a crash if there were active items!" });
		break;

	case DEBUG_ACTION_SPAWN_MONSTER:
	{
		std::string name = string_input_popup("Monster name (partial names OK):");
		Monster_type* type = MONSTER_TYPES.lookup_partial_name(name);
		if (!type)
		{
			messageQueue.addMessage({ Doryen::format("<c=dkgray>'{}' did not match any monsters.<c=/>", name) });
		}
		else
		{
			Monster* mon = new Monster(type);
			mon->pos = target_selector();
			entities.add_entity(mon);
		}
	}
		break;

	case DEBUG_ACTION_MONSTER_PATH:
		if (!entities.empty())
		{
			Entity* ent = *(entities.instances.begin());
			Monster* mon = static_cast<Monster*>(ent);
			mon->make_plans();
			std::stringstream path_info;
			path_info << "Monster: " << mon->pos.str() << std::endl;
			path_info << "You: " << player->pos.str() << std::endl;
			std::vector<Tripoint> path = mon->plan.path.get_points();
			path_info << "Path: (" << mon->plan.path.get_points().size() << ") " <<
					  std::endl;
			for (int i = 0; i < path.size(); i++)
			{
				path_info << path[i].str() << " => ";
			}
			popup(path_info.str().c_str());
		}
		break;

	case DEBUG_ACTION_MEMORY_INFO:
		popup("Sizeof(Submap): %d\nEntities: %d", sizeof(Submap), entities.instances.size());
		break;

	case DEBUG_ACTION_PLACE_BONUS:
	{
		Point p = map->get_center_point();
		p = worldmap->get_point(p.x, p.y);
		worldmap->add_bonus(p.x, p.y, true);  // true == debug on
	}
		break;

	case DEBUG_ACTION_GAIN_XP:
		player->gain_experience(100);
		break;

	default:
		messageQueue.addMessage({ Doryen::format("Nothing coded for {} yet.", debug_action_name(act)) });
	}
}

void PlayScreen::pickup_items(Tripoint pos)
{
// TODO: don't ignore z?
	pickup_items(pos.x, pos.y);
}

void PlayScreen::pickup_items(int posx, int posy)
{
	if (map->has_flag(TF_SEALED, posx, posy))
	{
		messageQueue.addMessage({ Doryen::format("<c=ltred>That {} is sealed; you cannot retrieve items there.<c=/>",
								  map->get_name(posx, posy)) });
		return;
	}
	if (!w_hud)
	{
		debugmsg("pickup_items() - w_hud is NULL!");
		return;
	}

	cuss::interface i_pickup;
	if (!i_pickup.load_from_file(CUSS_DIR + "/i_pickup.cuss"))
	{
		return;
	}

	int weight_after = player->current_weight(),
			volume_after = player->current_volume();
	std::vector<Item>* available = map->items_at(posx, posy);
	std::vector<bool> pick_up;
	std::vector<std::string> pickup_strings, weight_strings, volume_strings;
	for (int i = 0; i < available->size(); i++)
	{
		pick_up.push_back(false);
		weight_strings.push_back(itos((*available)[i].get_weight()));
		volume_strings.push_back(itos((*available)[i].get_volume()));
	}

	int offset = 0;
	cuss::element* ele_list_items = i_pickup.find_by_name("list_items");
	if (!ele_list_items)
	{
		debugmsg("No element 'list_items' in i_pickup.cuss");
		return;
	}
	int offset_size = ele_list_items->sizey;
	bool done = false;

	pickup_strings = get_pickup_strings(available, &pick_up);
// Set interface data
	i_pickup.set_data("weight_current", player->current_weight());
	i_pickup.set_data("weight_maximum", player->maximum_weight());
	i_pickup.set_data("volume_current", player->current_volume());
	i_pickup.set_data("volume_maximum", player->maximum_volume());
	for (int i = 0; i < offset_size && i < pickup_strings.size(); i++)
	{
		i_pickup.add_data("list_items", pickup_strings[i]);
		i_pickup.add_data("list_weight", weight_strings[i]);
		i_pickup.add_data("list_volume", volume_strings[i]);
	}

	while (!done)
	{
		i_pickup.set_data("weight_after", weight_after);
		i_pickup.set_data("volume_after", volume_after);
		i_pickup.draw(w_hud);
		long ch = getch();
		if (ch >= 'A' && ch <= 'Z')
		{
			ch = ch - 'A' + 'a'; // Convert uppercase letters to lowercase
		}
		if (ch >= 'a' && ch - 'a' < available->size())
		{
			int index = ch - 'a';
			pick_up[index] = !pick_up[index];
			bool pu = pick_up[index];
			Item* it = &((*available)[index]);
			pickup_strings[index] = pickup_string(it, ch, pu);
			i_pickup.set_data("list_items", pickup_strings);
			if (pu)
			{
				weight_after += it->get_weight();
				volume_after += it->get_volume();
			}
			else
			{
				weight_after -= it->get_weight();
				volume_after -= it->get_volume();
			}
			i_pickup.clear_data("list_items");
			i_pickup.clear_data("list_weight");
			i_pickup.clear_data("list_volume");
			std::stringstream weight_ss, volume_ss;
			weight_ss << (pu ? "<c=green>" : "<c=ltgray>") << it->get_weight() <<
					  "<c=/>";
			volume_ss << (pu ? "<c=green>" : "<c=ltgray>") << it->get_volume() <<
					  "<c=/>";
			weight_strings[index] = weight_ss.str();
			volume_strings[index] = volume_ss.str();
			for (int i = offset * offset_size;
				 i < (offset + 1) * offset_size && i < pickup_strings.size();
				 i++)
			{
				i_pickup.add_data("list_items", pickup_strings[i]);
				i_pickup.add_data("list_weight", weight_strings[i]);
				i_pickup.add_data("list_volume", volume_strings[i]);
			}
		}
		else if (ch == '<' && offset > 0)
		{
			offset--;
			i_pickup.clear_data("list_items");
			i_pickup.clear_data("list_weight");
			i_pickup.clear_data("list_volume");
			for (int i = offset * offset_size;
				 i < (offset + 1) * offset_size && i < pickup_strings.size();
				 i++)
			{
				i_pickup.add_data("list_items", pickup_strings[i]);
				i_pickup.add_data("list_weight", weight_strings[i]);
				i_pickup.add_data("list_volume", volume_strings[i]);
			}
		}
		else if (ch == '>' && available->size() > (offset + 1) * offset_size)
		{
			offset++;
			i_pickup.clear_data("list_items");
			i_pickup.clear_data("list_weight");
			i_pickup.clear_data("list_volume");
			for (int i = offset * offset_size;
				 i < (offset + 1) * offset_size && i < pickup_strings.size();
				 i++)
			{
				i_pickup.add_data("list_items", pickup_strings[i]);
				i_pickup.add_data("list_weight", weight_strings[i]);
				i_pickup.add_data("list_volume", volume_strings[i]);
			}
		}
		else if (ch == KEY_ESC)
		{
			return;
		}
		else if (ch == '\n')
		{
			done = true;
		}
	}
// TODO: Code for multi-turn pickup
	std::vector<Item> items_gotten;
	std::vector<bool> actually_got_it;
	for (int i = 0; i < available->size(); i++)
	{
		if (pick_up[i])
		{
			items_gotten.push_back((*available)[i]);
			if (player->add_item((*available)[i]))
			{
				actually_got_it.push_back(true);
			}
			else
			{
				actually_got_it.push_back(false);
			}
		}
		else
		{
			actually_got_it.push_back(false);
		}
	}

	for (int i = 0; i < actually_got_it.size(); i++)
	{
		if (actually_got_it[i])
		{
			available->erase(available->begin() + i);
			actually_got_it.erase(actually_got_it.begin() + i);
			i--;
		}
	}

	std::string message = "You pick up " + list_items(&items_gotten);
	messageQueue.addMessage({ message });
}

Tripoint PlayScreen::target_selector(int startx, int starty, int range,
		bool target_entities, bool show_path)
{
	std::vector<Tripoint> path = path_selector(startx, starty, range,
			target_entities, show_path);
	if (path.empty())
	{
		return Tripoint(-1, -1, -1);
	}
	return path.back();
}

std::vector<Tripoint> PlayScreen::path_selector(int startx, int starty, int range,
		bool target_entities, bool show_path)
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
		range = get_light_level();
	}
	if (range > get_light_level())
	{
		range = get_light_level();
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

// First draw; we need to draw the path since we might auto-target
	ret = map->line_of_sight(player->pos, target);
	map->draw_area(w_map, &entities, player->pos, minx, miny, maxx, maxy);
	if (show_path)
	{
		for (int i = 0; i < ret.size(); i++)
		{
			map->draw_tile(w_map, &entities, ret[i].x, ret[i].y,
					player->pos.x, player->pos.y, true); // true==inverted
		}
	}
	else
	{
// Just do the last tile
		map->draw_tile(w_map, &entities, ret.back().x, ret.back().y,
				player->pos.x, player->pos.y, true);
	}

// TODO: No no no remove this!  Won't work for tiles!
	Entity* ent_targeted = entities.entity_at(target);
	if (ent_targeted)
	{
		w_map->putglyph(w_map->sizex() / 2 - player->pos.x + target.x,
				w_map->sizey() / 2 - player->pos.y + target.y,
				ent_targeted->get_glyph().invert());
	}
	else
	{
		w_map->putglyph(w_map->sizex() / 2 - player->pos.x + target.x,
				w_map->sizey() / 2 - player->pos.y + target.y,
				glyph('*', c_red, c_black));
	}
	w_map->refresh();

	while (true)
	{
		long ch = input();
		bool redraw = false;
		if (ch == KEY_ESC || ch == 'q' || ch == 'Q')
		{
			return std::vector<Tripoint>();

		}
		else if ((ch == '<' || ch == '-') && !target_list.empty())
		{
			target_index--;
			if (target_index < 0)
			{
				target_index = target_list.size() - 1;
			}
			target = target_list[target_index];
			redraw = true;

		}
		else if ((ch == '>' || ch == '+' || ch == '=') && !target_list.empty())
		{
			target_index++;
			if (target_index >= target_list.size())
			{
				target_index = 0;
			}
			target = target_list[target_index];
			redraw = true;

		}
		else if (ch == '\n')
		{
			return ret;

		}
		else
		{
			Point p = input_direction(ch);
			if (p.x == 0 && p.y == 0)
			{
				return ret; // Return our path on hitting "pause"
			}
			else if (p.x != -2 && p.y != -2)
			{
				target += p;
// Ensure we're still in-range
				if (target.x < minx)
				{
					target.x = minx;
				}
				if (target.y < miny)
				{
					target.y = miny;
				}
				if (target.x > maxx)
				{
					target.x = maxx;
				}
				if (target.y > maxy)
				{
					target.y = maxy;
				}
				redraw = true;
			}
		}

		if (redraw)
		{
			ret = map->line_of_sight(player->pos, target);
			map->draw_area(w_map, &entities, player->pos, minx, miny, maxx, maxy);
			if (show_path)
			{
				for (int i = 0; i < ret.size(); i++)
				{
					map->draw_tile(w_map, &entities, ret[i].x, ret[i].y,
							player->pos.x, player->pos.y, true);// true==inverted
				}
			}
// TODO: No no no remove this!  Won't work for tiles!
			ent_targeted = entities.entity_at(target);
			if (ent_targeted)
			{
				w_map->putglyph(w_map->sizex() / 2 - player->pos.x + target.x,
						w_map->sizey() / 2 - player->pos.y + target.y,
						ent_targeted->get_glyph().invert());
			}
			else
			{
				w_map->putglyph(w_map->sizex() / 2 - player->pos.x + target.x,
						w_map->sizey() / 2 - player->pos.y + target.y,
						glyph('*', c_red, c_black));
			}
			w_map->refresh();
		}
	}
}

int PlayScreen::get_furniture_uid()
{
	return next_furniture_uid++;
}

int PlayScreen::get_light_level()
{
	int ret = timeManager.getTime().get_light_level();
	if (timeManager.getLightLevel() > ret)
	{
		return timeManager.getLightLevel();
	}
	return ret;
}

// UID defaults to -1
Tripoint PlayScreen::find_item(Item* it, int uid)
{
// Sanity check
	if (it == NULL && (uid < 0 || uid >= nextItemManager.getNextItemUid()))
	{
		return Tripoint(-1, -1, -1);
	}
// Check entities first - almost certainly faster than the map
	for (std::list<Entity*>::iterator iter = entities.instances.begin();
		 iter != entities.instances.end();
		 iter++)
	{
		if ((*iter)->has_item(it, uid))
		{
			return (*iter)->pos;
		}
	}
	Tripoint ret = map->find_item(it, uid);
	return ret;
}

Tripoint PlayScreen::find_item_uid(int uid)
{
	return find_item(NULL, uid);
}

std::vector<std::string>
get_pickup_strings(std::vector<Item>* items, std::vector<bool>* picking_up)
{
	std::vector<std::string> ret;
	if (items->size() != picking_up->size())
	{
		debugmsg("get_pickup_strings() - vectors aren't same size!");
		return ret;
	}

	for (int i = 0; i < items->size(); i++)
	{
		bool pickup = (*picking_up)[i];
		ret.push_back(pickup_string(&((*items)[i]), char('a' + i), pickup));
	}
	return ret;
}

std::string pickup_string(Item* item, char letter, bool picking_up)
{
	if (!item)
	{
		return "<c=red>NO ITEM<c=/>";
	}
	std::stringstream ss;
	ss << "<c=" << (picking_up ? "green" : "ltgray") << ">" << letter <<
	   (picking_up ? " +" : " -") << " " << item->get_name_full();
	return ss.str();
}

void PlayScreen::draw()
{
	// Print everything (update_hud() and map->draw())
	if (!player->has_activity())
	{
		draw_all();
	}
}

void PlayScreen::updated()
{
	// Reset all temp values.
	reset_temp_values();
	// Process active items; these may set temp values!
	process_active_items();

	/* TODO:  It's be nice to move all of this to Player::take_turn().  Then we
	 *        won't have to special case it - it'd just be another entity taking
	 *        their turn!
	 */

	// Give the player their action points
	player->gain_action_points();
	// Set all values, incur hunger/thirst, etc.
	player->start_turn();
}

Cataclysm::ScreenType PlayScreen::processInput()
{
	// Sanity check
	if (!w_map || !w_hud || !player || !worldmap || !map)
	{
		return Cataclysm::ScreenType::QUIT;
	}
	if (game_over)
	{
		return Cataclysm::ScreenType::QUIT;
	}

	while (player->action_points > 0)
	{
		// Handle the player's activity (e.g. reloading, crafting, etc)
		handle_player_activity();
		// Update the map in case we need to right now
		shift_if_needed();

		// The player doesn't get to give input if they have an active activity.
		if (!player->activity.is_active())
		{
			long ch = input();
			// Fetch the action bound to whatever key we pressed...
			Interface_action act = KEYBINDINGS.bound_to_key(ch);
			// ... and do that action.
			Cataclysm::ScreenType action = do_action(act);

			// If the action to make not is null
			if (action not_eq Cataclysm::ScreenType::NONE) return action;
		}
	}
	// Map processes fields after the player
	map->process_fields();
	// Shift the map - it's likely that the player moved or something
	shift_if_needed();
	// Now all other entities get their turn
	move_entities();
	// Maybe a monster killed us
	if (game_over)
	{
		return Cataclysm::ScreenType::QUIT;
	}
	// Advance the turn
	timeManager.getTime().increment();
	// This keeps the game going
	return Cataclysm::ScreenType::NONE;
}
