// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/files.h>
#include <Cataclysm/Entity/Player/StateInventory.hpp>
#include "Cataclysm/Screen/InformationItemScreen.hpp"

using namespace Cataclysm;

InformationItemScreen::InformationItemScreen()
{
	if (!i_info.load_from_file(CUSS_DIR + "/i_item_info.cuss"))
	{
		throw std::runtime_error("Cannot found i_item_info.cuss");
	}
}

void InformationItemScreen::draw()
{
	i_info.draw(console);

	console.draw();
}

void InformationItemScreen::updated()
{
	Item item = stateInventory.getItemSelected();

	i_info.set_data("item_name", item.get_name_full());
	i_info.set_data("num_weight", item.get_weight());
	i_info.set_data("num_volume", item.get_volume());
	i_info.set_data("num_bash", item.get_damage(DAMAGE_BASH));
	i_info.set_data("num_cut", item.get_damage(DAMAGE_CUT));
	i_info.set_data("num_pierce", item.get_damage(DAMAGE_PIERCE));
	i_info.set_data("num_to_hit", item.get_to_hit());
	i_info.set_data("num_speed", item.get_base_attack_speed());

	std::stringstream actions;

	app_actions = item.get_applicable_actions();

	for (int i = 0; i < app_actions.size(); i++)
	{
		switch (app_actions[i])
		{
		case IACT_WIELD:
			actions << "<c=magenta>w<c=/>ield" << std::endl;
			break;
		case IACT_WEAR:
			actions << "<c=magenta>W<c=/>ear" << std::endl;
			break;
		case IACT_DROP:
			actions << "<c=magenta>d<c=/>rop" << std::endl;
			break;
		case IACT_EAT:
			actions << "<c=magenta>e<c=/>at" << std::endl;
			break;
		case IACT_APPLY:
			actions << "<c=magenta>a<c=/>pply" << std::endl;
			break;
		case IACT_READ:
			actions << "<c=magenta>r<c=/>ead" << std::endl;
			break;
		case IACT_UNLOAD:
			actions << "<c=magenta>U<c=/>nload" << std::endl;
			break;
		case IACT_RELOAD:
			actions << "<c=magenta>R<c=/>eload" << std::endl;
			break;
		case IACT_BUTCHER:
			actions << "<c=magenta>B<c=/>utcher" << std::endl;
			break;
		}
	}

	actions << std::endl << "<c=magenta>Esc<c=/> or <c=magenta>q<c=/>: " << "Cancel / Do nothing";

	i_info.set_data("text_actions", actions.str());
	// Includes type-specific info, e.g. nutrition for food
	i_info.set_data("description", item.get_description_full());
}

ScreenType InformationItemScreen::processInput()
{
	flushinp();

	const long ch = getch();

	Item_action ret = IACT_NULL;

	switch (ch)
	{
	case 'd':
	case 'D':
		ret = IACT_DROP;
		break;
	case 'w':
		ret = IACT_WIELD;
		break;
	case 'W':
		ret = IACT_WEAR;
		break;
	case 'e':
		ret = IACT_EAT;
		break;
	case 'a':
		ret = IACT_APPLY;
		break;
	case 'r':
		ret = IACT_READ;
		break;
	case 'U':
		ret = IACT_UNLOAD;
		break;
	case 'R':
		ret = IACT_RELOAD;
		break;
	case 'B':
		ret = IACT_BUTCHER;
		break;

	case KEY_ESC:
	case 'q':
	case 'Q':
		return ScreenType::PLAY;
	}

	if (ret != IACT_NULL)
	{ // We chose an action.

		// Check the list of applicable actions to see if the action the player chose is
		// actually available.
		for (int i = 0; i < app_actions.size(); i++)
		{
			if (app_actions[i] == ret)
			{
//				return ret;
			}
		}
	}

	return ScreenType::NONE;
}
