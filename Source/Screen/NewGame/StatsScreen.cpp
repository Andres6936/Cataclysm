// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/files.h>
#include "Cataclysm/Screen/NewGame/StatsScreen.hpp"

using namespace Cataclysm;

void StatsScreen::draw()
{
	INewGameScreen::draw();
}

void StatsScreen::updated()
{
	filenameInterfaceCuss = CUSS_DIR + "/i_newchar_stats.cuss";

	INewGameScreen::updated();

	cur_stat = Stat_selected::STATSEL_STR;
	i_newch.set_data("text_strength", "<c=ltblue>Strength<c=/>");
	i_newch.set_data("text_dexterity", "<c=ltgray>Dexterity<c=/>");
	i_newch.set_data("text_perception", "<c=ltgray>Perception<c=/>");
	i_newch.set_data("text_intelligence", "<c=ltgray>Intelligence<c=/>");
	i_newch.ref_data("num_strength", &player->stats.strength);
	i_newch.ref_data("num_dexterity", &player->stats.dexterity);
	i_newch.ref_data("num_perception", &player->stats.perception);
	i_newch.ref_data("num_intelligence", &player->stats.intelligence);
	i_newch.set_data("text_description", getStatDescription(cur_stat));
}

ScreenType StatsScreen::processInput()
{
	flushinp();
	long ch = getch();

	if (ch == '>')
	{
		return ScreenType::NEW_GAME_STATS;
	}
	else if (ch == '<')
	{
		return ScreenType::NEW_GAME_CANCEL;
	}

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

	return ScreenType::NONE;
}
