// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/files.h>
#include <Cataclysm/Util/globals.h>
#include "Cataclysm/Screen/NewGame/Interface/INewGameScreen.hpp"

using namespace Cataclysm;

INewGameScreen::INewGameScreen()
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

	stat_value = &(player->stats.strength);

	player->set_profession(PROFESSIONS.random_instance());
}


std::vector<std::string> INewGameScreen::getTraitList()
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

std::vector<std::string> INewGameScreen::getProfessionList()
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

std::string INewGameScreen::getStatDescription(Stat_selected stat)
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

void INewGameScreen::prevScreen()
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
		break;
	}

	userChangedScreen = true;
}

void INewGameScreen::nextScreen()
{
	switch (cur_screen)
	{

	case New_char_screen::NCS_CANCEL:
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

void INewGameScreen::prevStat()
{
	switch (cur_stat)
	{

	case Stat_selected::STATSEL_STR:
		cur_stat = Stat_selected::STATSEL_INT;
		break;

	case Stat_selected::STATSEL_DEX:
		cur_stat = Stat_selected::STATSEL_STR;
		break;

	case Stat_selected::STATSEL_PER:
		cur_stat = Stat_selected::STATSEL_DEX;
		break;

	case Stat_selected::STATSEL_INT:
		cur_stat = Stat_selected::STATSEL_PER;
		break;
	}
}

void INewGameScreen::nextStat()
{
	switch (cur_stat)
	{

	case Stat_selected::STATSEL_STR:
		cur_stat = Stat_selected::STATSEL_DEX;
		break;

	case Stat_selected::STATSEL_DEX:
		cur_stat = Stat_selected::STATSEL_PER;
		break;

	case Stat_selected::STATSEL_PER:
		cur_stat = Stat_selected::STATSEL_INT;
		break;

	case Stat_selected::STATSEL_INT:
		cur_stat = Stat_selected::STATSEL_STR;
		break;
	}
}

void INewGameScreen::verifyInvariantPlayer()
{
	// Now set up our skills and equipment based on our profession
	if (!player->profession)
	{
		throw std::runtime_error("Character creation finished without a profession!");
	}

	std::vector<Item_type_chance> prof_items = player->profession->items.item_types;
	for (int i = 0; i < prof_items.size(); i++)
	{
		int count = prof_items[i].number;
		Item tmp_it(prof_items[i].item);
		for (int i = 0; i < count; i++)
		{
			if (tmp_it.get_item_class() == ITEM_CLASS_CLOTHING)
			{
				player->items_worn.push_back(tmp_it);
			}
			else
			{
				player->inventory.push_back(tmp_it);
			}
		}
	}

	player->skills = player->profession->skills;

	// The "Durable" trait needs to be set up here.
	if (player->has_trait(TRAIT_DURABLE))
	{
		for (int i = 0; i < HP_PART_MAX; i++)
		{
			player->current_hp[i] = 115;
			player->max_hp[i] = 115;
		}
	}

	// Set up our max mental skill levels, based on int.
	int max_sk = player->stats.intelligence / 5;
	for (int i = 0; i < SKILL_MAX; i++)
	{
		Skill_type sk = Skill_type(i);
		// If we start with some skill from our profession, increase the max by that
		int cap = max_sk + player->skills.get_level(sk);
		if (is_skill_mental(sk))
		{
			player->skills.set_max_level(sk, cap);
		}
	}

	// Myopic characters get free glasses
	if (player->has_trait(TRAIT_MYOPIC))
	{
		Item_type* glasses = ITEM_TYPES.lookup_name("glasses");
		if (!glasses)
		{
			throw std::logic_error("No item 'glasses' exists - required for the Myopic trait!");
		}
		Item tmp_it(glasses);
		player->items_worn.push_back(tmp_it);
	}

	// Set max stats to current stats.
	player->stats.strength_max = player->stats.strength;
	player->stats.dexterity_max = player->stats.dexterity;
	player->stats.perception_max = player->stats.perception;
	player->stats.intelligence_max = player->stats.intelligence;

	// Assign some starting missions!
	player->assign_personal_missions();

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
}

void INewGameScreen::draw()
{
	i_newch.draw(newGameConsole);

	newGameConsole.blit({0,0}, console, {0, 0});
	newGameConsole.draw();
}

void INewGameScreen::updated()
{
	// Reference for avoid unnecessary load
	static std::string reference;

	if (reference != filenameInterfaceCuss)
	{
		if (!i_newch.load_from_file(filenameInterfaceCuss))
		{
			throw std::runtime_error("Cannot found the file: " + filenameInterfaceCuss + "\n");
		}

		i_newch.ref_data("num_points", &points);

		// Update the reference
		reference = filenameInterfaceCuss;
	}
}

ScreenType INewGameScreen::processInput()
{
	return ScreenType::NONE;
}
