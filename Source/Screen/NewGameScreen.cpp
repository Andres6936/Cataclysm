// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/NewGameScreen.hpp"
#include <Cataclysm/files.h>
#include <Cataclysm/globals.h>

using namespace Cataclysm;

NewGameScreen::NewGameScreen()
{
	worldmap = std::make_unique<Worldmap>();

	if (indexWorldSelect >= 0 && indexWorldSelect < worldmap_names.size())
	{
		std::string world_file = SAVE_DIR + "/worlds/" +
								 worldmap_names[indexWorldSelect] + ".map";
		if (!worldmap->load_from_file(world_file))
		{
			throw std::runtime_error(Doryen::format("Couldn't load worldmap from '{}'.", world_file));
		}
	}
	else
	{
		worldmap->generate();
	}

	// Need to set time BEFORE creating a new character - because creating a new
	// character uses time to set mission deadlines!
	time = Time(0, 0, 8, 1, SEASON_SPRING, STARTING_YEAR);

	map = std::make_unique<Map>();
	player = std::make_unique<Player>();

	player->prep_new_character();

	// Start Create New Character

	if (!i_newch.load_from_file(CUSS_DIR + "/i_newchar_stats.cuss"))
	{
		throw std::runtime_error("Cannot found the file i_newchar_stats.cuss");
	}

	for (int i = 1; i < TRAIT_MAX_BAD; i++)
	{
		if (i != TRAIT_MAX_GOOD && i != TRAIT_MAX_NEUTRAL)
		{
			selectable_traits.push_back(Trait_id(i));
		}
	}

	traits_list = getTraitList();
	profession_list = getProfessionList();

	i_newch.ref_data("num_points", &points);

	i_newch.ref_data("num_strength", &player->stats.strength);
	i_newch.ref_data("num_dexterity", &player->stats.dexterity);
	i_newch.ref_data("num_perception", &player->stats.perception);
	i_newch.ref_data("num_intelligence", &player->stats.intelligence);
	i_newch.set_data("text_description", getStatDescription(cur_stat));

	i_newch.set_data("text_strength", "<c=ltblue>Strength<c=/>");
	i_newch.set_data("text_dexterity", "<c=ltgray>Dexterity<c=/>");
	i_newch.set_data("text_perception", "<c=ltgray>Perception<c=/>");
	i_newch.set_data("text_intelligence", "<c=ltgray>Intelligence<c=/>");


	// End Create New Character

	if (!player->create_new_character())
	{
		userCreatedPlayer = false;
	}

	// entities[0] should always be the player!
	entities.add_entity(player.get());

	// The second argument of 0 means "on the main island"
	Point start = worldmap->random_tile_with_terrain("beach", 0);

	// Set the starting point to a shipwreck beach!
	worldmap->set_terrain(start.x, start.y, "beach_shipwreck");

	// Prep our Submap_pool.
	SUBMAP_POOL.load_area_centered_on(start.x, start.y);

	// And then generate our map.
	map->generate(worldmap.get(), start.x, start.y, 0);

	/*
	if (TESTING_MODE) {
	debugmsg("Pool covers %s, map covers %s.",
			 SUBMAP_POOL.get_range_text().c_str(),
			 map->get_range_text().c_str());
	}
	*/
	worldmap->set_terrain(start.x, start.y, "beach");

	userCreatedPlayer = true;
}

void NewGameScreen::draw()
{
	i_newch.draw(newGameConsole);

	newGameConsole.blit({0,0}, console, {0, 0});
	newGameConsole.draw();
}

void NewGameScreen::updated()
{

}

ScreenType NewGameScreen::processInput()
{
	// Exit of Scene if the user has been created successfully a player
	if (userCreatedPlayer) return ScreenType::PLAY;

	// Reset the state of screens
	userChangedScreen = false;

	long ch = getch();

	if (ch == '<')
	{
		prevScreen();

		if (cur_screen == New_char_screen::NCS_CANCEL)
		{
			if (showQueryYesNo("Cancel character creation?"))
			{
				return ScreenType::MENU;
			}
			else
			{
				cur_screen = New_char_screen::NCS_STATS;
			}
		}
	}
	else if (ch == '>')
	{
		nextScreen();

		if (cur_screen == New_char_screen::NCS_DONE)
		{
			std::string reason_for_fail;

			if (points > 0)
			{
				reason_for_fail += "\nYou have unspent points!";
			}
			if (player->profession == nullptr)
			{
				reason_for_fail += "\nYou didn't choose a profession!";
			}
			if (player->name.empty())
			{
				reason_for_fail += "\nYour name is blank!";
			}

			if (!reason_for_fail.empty())
			{
				showDebugMessage(Doryen::format("Wait, you can't start the game yet! {}", reason_for_fail));
			}
			else if (showQueryYesNo("Complete character and start the game?"))
			{
				return ScreenType::PLAY;
			}

			cur_screen = New_char_screen::NCS_DESCRIPTION;
		}
	}
	else
	{
		switch (cur_screen)
		{

		case New_char_screen::NCS_CANCEL:
			break;

		case New_char_screen::NCS_STATS:
		{
			bool changed_stat = false;

			switch (ch)
			{
			case '2':
			case 'j':
			case KEY_DOWN:
				if (cur_stat == Stat_selected::STATSEL_INT)
				{
					cur_stat = Stat_selected::STATSEL_STR;
				}
				else
				{
					nextStat();
				}
				changed_stat = true;
				break;

			case '8':
			case 'k':
			case KEY_UP:
				if (cur_stat == Stat_selected::STATSEL_STR)
				{
					cur_stat = Stat_selected::STATSEL_INT;
				}
				else
				{
					prevStat();
				}
				changed_stat = true;
				break;

			case '4':
			case 'h':
			case KEY_LEFT:
				if (*stat_value > 4)
				{
					if (*stat_value > 16)
					{
						points++; // Stats above 16 cost 2 points, so get extra back
					}
					points++;
					(*stat_value)--;
				}
				break;

			case '6':
			case 'l':
			case KEY_RIGHT:
			{
				int point_req = (*stat_value >= 16 ? 2 : 1);
				if (*stat_value < 20 && points >= point_req)
				{
					points -= point_req;
					(*stat_value)++;
				}
			}
				break;
			} // switch (ch)

			if (changed_stat)
			{
				// Update stat_value
				i_newch.set_data("text_strength", "<c=ltgray>Strength<c=/>");
				i_newch.set_data("text_dexterity", "<c=ltgray>Dexterity<c=/>");
				i_newch.set_data("text_perception", "<c=ltgray>Perception<c=/>");
				i_newch.set_data("text_intelligence", "<c=ltgray>Intelligence<c=/>");

				i_newch.set_data("text_description",
						getStatDescription(cur_stat));
				switch (cur_stat)
				{
				case Stat_selected::STATSEL_STR:
					stat_value = &(player->stats.strength);
					i_newch.set_data("text_strength",
							"<c=ltblue>Strength<c=/>");
					break;

				case Stat_selected::STATSEL_DEX:
					stat_value = &(player->stats.dexterity);
					i_newch.set_data("text_dexterity",
							"<c=ltblue>Dexterity<c=/>");
					break;

				case Stat_selected::STATSEL_PER:
					stat_value = &(player->stats.perception);
					i_newch.set_data("text_perception",
							"<c=ltblue>Perception<c=/>");
					break;

				case Stat_selected::STATSEL_INT:
					stat_value = &(player->stats.intelligence);
					i_newch.set_data("text_intelligence",
							"<c=ltblue>Intelligence<c=/>");
					break;
				}
			}
		}
			break;

		case New_char_screen::NCS_TRAITS:
			switch (ch)
			{
			case '2':
			case 'j':
			case KEY_DOWN:
			{
				i_newch.add_data("list_traits", 1);
				int sel = i_newch.get_int("list_traits");
				Trait_id cur_trait = selectable_traits[sel];
				i_newch.set_data("num_cost", abs(trait_cost(cur_trait)));
				if (trait_cost(cur_trait) >= 0)
				{
					i_newch.set_data("text_cost", "<c=yellow>Cost:<c=/>");
				}
				else
				{
					i_newch.set_data("text_cost", "<c=yellow>Earns:<c=/>");
				}
				if (trait_cost(cur_trait) > points)
				{
					i_newch.set_data("num_cost", c_red);
				}
				else
				{
					i_newch.set_data("num_cost", c_white);
				}
				i_newch.set_data("text_description",
						trait_description(cur_trait));
			}
				break;

			case '8':
			case 'k':
			case KEY_UP:
			{
				i_newch.add_data("list_traits", -1);
				int sel = i_newch.get_int("list_traits");
				Trait_id cur_trait = selectable_traits[sel];
				i_newch.set_data("num_cost", abs(trait_cost(cur_trait)));
				if (trait_cost(cur_trait) >= 0)
				{
					i_newch.set_data("text_cost", "<c=yellow>Cost:<c=/>");
				}
				else
				{
					i_newch.set_data("text_cost", "<c=yellow>Earns:<c=/>");
				}
				if (trait_cost(cur_trait) > points)
				{
					i_newch.set_data("num_cost", c_red);
				}
				else
				{
					i_newch.set_data("num_cost", c_white);
				}
				i_newch.set_data("text_description",
						trait_description(cur_trait));
			}
				break;

			case '\n':
			case ' ':
			{
				int sel = i_newch.get_int("list_traits");
				Trait_id cur_trait = selectable_traits[sel];
				if (player->has_trait(cur_trait))
				{
					player->traits[cur_trait] = false;
					points += trait_cost(cur_trait);
					num_traits--;
					traits_list = getTraitList();
				}
				else if (points >= trait_cost(cur_trait) && num_traits < 5)
				{
					player->traits[cur_trait] = true;
					points -= trait_cost(cur_trait);
					num_traits++;
					traits_list = getTraitList();
				}
				i_newch.set_data("num_traits_left", 5 - num_traits);
				if (num_traits >= 5)
				{
					i_newch.set_data("num_traits_left", c_red);
				}
			}
				break;

			} // switch (ch)
			break;

		case New_char_screen::NCS_PROFESSION:
			break;

		case New_char_screen::NCS_DESCRIPTION:
			break;

		case New_char_screen::NCS_DONE:
			break;
		}
	}
}

std::vector<std::string> NewGameScreen::getTraitList()
{
	std::vector<std::string> ret;

	for (int i = 1; i < TRAIT_MAX_BAD; i++)
	{
		// Skip over "marker" traits
		if (i == TRAIT_MAX_GOOD || i == TRAIT_MAX_NEUTRAL)
		{
			i++;
		}

		std::stringstream name;

		if (player->has_trait(Trait_id(i)))
		{
			name << "<c=white>";
		}
		else if (i < TRAIT_MAX_GOOD)
		{
			name << "<c=green>";
		}
		else if (i < TRAIT_MAX_NEUTRAL)
		{
			name << "<c=brown>";
		}
		else
		{
			name << "<c=red>";
		}

		name << trait_id_name(Trait_id(i)) << "<c=/>";

		ret.push_back(name.str());
	}

	return ret;
}

std::vector<std::string> NewGameScreen::getProfessionList()
{
	std::vector<std::string> ret;

	for (std::list<Profession*>::iterator it = PROFESSIONS.instances.begin();
		 it != PROFESSIONS.instances.end();
		 it++)
	{
		std::stringstream text;

		if ((*it) == player->get_profession())
		{
			text << "<c=white>";
		}
		else
		{
			text << "<c=ltblue>";
		}

		text << (*it)->name << "<c=/>";

		ret.push_back(text.str());
	}

	return ret;
}

std::string NewGameScreen::getStatDescription(Stat_selected stat)
{
	switch (stat)
	{

	case Stat_selected::STATSEL_STR:
		return "\
Strength affects the amount of weight you can carry or drag, and your base \
melee damage.  It's especially important for increasing the damage when using \
a blunt weapon.  It also helps you hit faster when using a heavy weapon. Many \
actions which require brute force, like smashing through doors, rely on \
strength.";

	case Stat_selected::STATSEL_DEX:
		return "\
Dexterity improves your chance of hitting a target in melee combat, and to a \
lesser degree, ranged combat.  It improves damage when using a piercing \
weapon, and improves speed when attacking with a large object.  It's also \
useful for a variety of smaller purposes, like falling gracefully or avoiding \
traps.";

	case Stat_selected::STATSEL_PER:
		return "\
Perception is vital for accurate ranged attacks.  It slightly improves your \
damage with cutting or piercing melee weapons.  Finally, it's useful for a \
variety of minor uses, like detecting traps or stealthy monsters, noticing \
when an NPC is lying, or smuggling items.";

	case Stat_selected::STATSEL_INT:
		return "\
Intelligence is the most subtle stat, but often the most important.  It \
affects how quickly you can read books, and how well you'll absorb their \
contents.  It's also important for NPC interaction and the use of bionics.";
	}

	return "Unknown stat???";
}

void NewGameScreen::prevScreen()
{
	switch (cur_screen)
	{

	case New_char_screen::NCS_CANCEL:
		break;

	case New_char_screen::NCS_STATS:
		cur_screen = New_char_screen::NCS_CANCEL;
		break;

	case New_char_screen::NCS_TRAITS:
		cur_screen = New_char_screen::NCS_STATS;
		break;

	case New_char_screen::NCS_PROFESSION:
		cur_screen = New_char_screen::NCS_TRAITS;
		break;

	case New_char_screen::NCS_DESCRIPTION:
		cur_screen = New_char_screen::NCS_PROFESSION;
		break;

	case New_char_screen::NCS_DONE:
		cur_screen = New_char_screen::NCS_DESCRIPTION;
		break;
	}

	userChangedScreen = true;
}

void NewGameScreen::nextScreen()
{
	switch (cur_screen)
	{

	case New_char_screen::NCS_CANCEL:
		cur_screen = New_char_screen::NCS_STATS;
		break;

	case New_char_screen::NCS_STATS:
		cur_screen = New_char_screen::NCS_TRAITS;
		break;

	case New_char_screen::NCS_TRAITS:
		cur_screen = New_char_screen::NCS_PROFESSION;
		break;

	case New_char_screen::NCS_PROFESSION:
		cur_screen = New_char_screen::NCS_DESCRIPTION;
		break;

	case New_char_screen::NCS_DESCRIPTION:
		cur_screen = New_char_screen::NCS_DONE;
		break;

	case New_char_screen::NCS_DONE:
		break;
	}

	userChangedScreen = true;
}

void NewGameScreen::prevStat()
{
	switch (cur_stat)
	{

	case Stat_selected::STATSEL_STR:
		break;

	case Stat_selected::STATSEL_DEX:
		break;

	case Stat_selected::STATSEL_PER:
		break;

	case Stat_selected::STATSEL_INT:
		break;
	}
}

void NewGameScreen::nextStat()
{
	switch (cur_stat)
	{

	case Stat_selected::STATSEL_STR:
		break;

	case Stat_selected::STATSEL_DEX:
		break;

	case Stat_selected::STATSEL_PER:
		break;

	case Stat_selected::STATSEL_INT:
		break;
	}
}
