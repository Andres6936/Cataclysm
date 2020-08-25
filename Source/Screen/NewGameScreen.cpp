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
	// Player::create_new_character() returns false if the user cancels the process.
	if (!player->create_new_character())
	{
		userCreatedPlayer = false;
	}

	// entities[0] should always be the player!
	entities.add_entity(player);

	// The second argument of 0 means "on the main island"
	Point start = worldmap->random_tile_with_terrain("beach", 0);

	// Set the starting point to a shipwreck beach!
	worldmap->set_terrain(start.x, start.y, "beach_shipwreck");

	// Prep our Submap_pool.
	SUBMAP_POOL.load_area_centered_on(start.x, start.y);

	// And then generate our map.
	map->generate(worldmap, start.x, start.y, 0);

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

}

void NewGameScreen::updated()
{

}

ScreenType NewGameScreen::processInput()
{
	if (userCreatedPlayer)
	{
		return ScreenType::PLAY;
	}
	else
	{
		return ScreenType::MENU;
	}
}
