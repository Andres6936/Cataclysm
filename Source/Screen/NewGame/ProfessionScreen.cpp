// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/files.h>
#include <Cataclysm/globals.h>
#include <Cataclysm/Util/String/stringfunc.h>
#include "Cataclysm/Screen/NewGame/ProfessionScreen.hpp"

using namespace Cataclysm;

void ProfessionScreen::draw()
{
	INewGameScreen::draw();
}

void ProfessionScreen::updated()
{
	filenameInterfaceCuss = CUSS_DIR + "/i_newchar_profession.cuss";

	INewGameScreen::updated();

	i_newch.select("list_professions");
	i_newch.ref_data("list_professions", &profession_list);
	std::string prof_name = i_newch.get_str("list_professions");
	prof_name = remove_color_tags(prof_name);
	Profession* cur_prof = PROFESSIONS.lookup_name(prof_name);
	if (!cur_prof)
	{
		throw std::runtime_error(Doryen::format("No such profession as '{}'!", prof_name));
	}
	i_newch.set_data("text_description", cur_prof->description);
}

ScreenType ProfessionScreen::processInput()
{
	flushinp();
	long ch = getch();

	if (ch == '>')
	{
		return ScreenType::NEW_GAME_DESCRIPTION;
	}
	else if (ch == '<')
	{
		return ScreenType::NEW_GAME_TRAITS;
	}

	switch (ch)
	{
	case '2':
	case 'j':
	case KEY_DOWN:
	{
		i_newch.add_data("list_professions", 1);
		std::string prof_name = i_newch.get_str("list_professions");
		prof_name = remove_color_tags(prof_name);
		Profession* cur_prof = PROFESSIONS.lookup_name(prof_name);

		if (!cur_prof)
		{
			showDebugMessage(Doryen::format("No such profession as '{}'!", prof_name));
			return ScreenType::MENU;
		}

		i_newch.set_data("text_description", cur_prof->description);
	}
		break;

	case '8':
	case 'k':
	case KEY_UP:
	{
		i_newch.add_data("list_professions", -1);
		std::string prof_name = i_newch.get_str("list_professions");
		prof_name = remove_color_tags(prof_name);
		Profession* cur_prof = PROFESSIONS.lookup_name(prof_name);
		if (!cur_prof)
		{
			showDebugMessage(Doryen::format("No such profession as '{}'!", prof_name));
			return ScreenType::MENU;
		}
		i_newch.set_data("text_description", cur_prof->description);
	}
		break;

	case '\n':
	case ' ':
	{
		std::string prof_name = i_newch.get_str("list_professions");
		prof_name = remove_color_tags(prof_name);
		Profession* cur_prof = PROFESSIONS.lookup_name(prof_name);
		if (!cur_prof)
		{
			showDebugMessage(Doryen::format("No such profession as '{}'!", prof_name));
			return ScreenType::MENU;
		}

		player->set_profession(cur_prof);
		profession_list = getProfessionList();
	}
		break;

	} // switch (ch)

	return ScreenType::NONE;
}
