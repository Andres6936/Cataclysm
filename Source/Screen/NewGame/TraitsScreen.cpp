// Joan Andrés (@Andres6936) Github.

#include "Cataclysm/Screen/NewGame/TraitsScreen.hpp"

using namespace Cataclysm;

void TraitsScreen::draw()
{
	INewGameScreen::draw();
}

void TraitsScreen::updated()
{
	INewGameScreen::updated();

	i_newch.select("list_traits");
	i_newch.ref_data("list_traits", &traits_list);
	int sel = i_newch.get_int("list_traits");
	Trait_id cur_trait = selectable_traits[sel];
	i_newch.set_data("text_description", trait_description(cur_trait));
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
	i_newch.set_data("num_traits_left", 5 - num_traits);
	if (num_traits >= 5)
	{
		i_newch.set_data("num_traits_left", c_red);
	}
}

ScreenType TraitsScreen::processInput()
{
	flushinp();
	long ch = getch();

	if (ch == '>')
	{
		return ScreenType::NEW_GAME_PROFESSION;
	}
	else if (ch == '<')
	{
		return ScreenType::NEW_GAME_STATS;
	}

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


	return ScreenType::NONE;
}
