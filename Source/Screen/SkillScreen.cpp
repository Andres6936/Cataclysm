// Joan Andrés (@Andres6936) Github.

#include <Cataclysm/Util/files.h>
#include "Cataclysm/Screen/SkillScreen.hpp"

using namespace Cataclysm;

SkillScreen::SkillScreen()
{
	std::string iface_file = CUSS_DIR + "/i_char_skills.cuss";

	if (!i_skills.load_from_file(iface_file))
	{
		throw std::runtime_error("Cannot found the file i_char_skills.cuss");
	}
}

void SkillScreen::draw()
{
	i_skills.draw(skillScreen);

	skillScreen.blit({0, 0}, console, {0, 0});
	skillScreen.draw();
}

void SkillScreen::updated()
{
	std::string cur_name = "list_name_1",
			cur_level = "list_level_1",
			cur_max = "list_max_1",
			cur_cost = "list_cost_1";

	i_skills.clear_data("list_name_1");
	i_skills.clear_data("list_level_1");
	i_skills.clear_data("list_max_1");
	i_skills.clear_data("list_cost_1");
	i_skills.clear_data("list_name_2");
	i_skills.clear_data("list_level_2");
	i_skills.clear_data("list_max_2");
	i_skills.clear_data("list_cost_2");

	int list_size = i_skills.element_height(cur_name);
	if (list_size == -1)
	{
		showDebugMessage("i_char_skills.cuss is missing element 'list_name_1'!");
		return;
	}

// Set up the lists.
	for (int i = 1; i < SKILL_MAX; i++)
	{

		Skill_type sk = Skill_type(i);
		int cost = player->skills.improve_cost(sk);
		if (player->skills.maxed_out(sk) && player->has_trait(TRAIT_AUTODIDACT))
		{
			cost *= 2;
		}
		bool maxed = (player->skills.maxed_out(sk) && !player->has_trait(TRAIT_AUTODIDACT));

		std::stringstream sk_name, sk_level, sk_max, sk_cost;
		sk_name << "<c=pink>";
		if (i <= 26)
		{
			sk_name << char(i - 1 + 'a');
		}
		else
		{
			sk_name << char(i - 27 + 'A');
		}
		sk_name << "<c=/>: ";
		if (!maxed && player->experience >= cost)
		{
			sk_name << "<c=white>";
		}
		sk_name << skill_type_user_name(sk) << "<c=/>";
		i_skills.add_data(cur_name, sk_name.str());

		if (player->skills.get_level(sk) == 0)
		{
			sk_level << "<c=dkgray>";
		}
		else if (maxed)
		{
			sk_level << "<c=red>";
			sk_max << "<c=red>";
		}
		sk_level << player->skills.get_level(sk) << "<c=/> /";
		i_skills.add_data(cur_level, sk_level.str());

		if (player->skills.is_unlocked(sk))
		{
			sk_max << "<c=dkgray>--<c=/>";
		}
		else
		{
			sk_max << player->skills.get_max_level(sk) << "<c=/>";
		}
		i_skills.add_data(cur_max, sk_max.str());

		if (player->experience >= cost)
		{
			sk_cost << "<c=white>";
		}
		else
		{
			sk_cost << "<c=dkgray>";
		}
		sk_cost << cost << "<c=/>";
		i_skills.add_data(cur_cost, sk_cost.str());

// Switch to the next list if needed
		if (i == list_size)
		{
			cur_name = "list_name_2";
			cur_level = "list_level_2";
			cur_max = "list_max_2";
			cur_cost = "list_cost_2";
		}
	}

	i_skills.set_data("num_experience", player->experience);

	i_skills.set_data("text_help", "\
<c=yellow>Press the letter attached to a skill to improve it.  You cannot \
improve a skill beyond its cap.\n\n\
Press <c=pink>?<c=yellow> and then a letter for skill description.<c=/>");
}

ScreenType SkillScreen::processInput()
{
	flushinp();
	long ch = getch();

	if (ch == KEY_ESC)
	{
		return ScreenType::PLAY;
	}
	else if (ch == '1')
	{
		return ScreenType::STATUS;

	}
	else if (ch == '3')
	{
		return ScreenType::CLOTHING;

	}
	else if (ch == '4')
	{
		return ScreenType::MISSIONS;

	}
	else if (ch == '?')
	{
		if (helpMode)
		{
			return ScreenType::HELP_MENU;
		}
		helpMode = !helpMode;

		if (helpMode)
		{
			i_skills.set_data("text_help", "\
<c=yellow>Press the letter attached to a skill for information on that skill.\
\n\n\
Press <c=pink>?<c=yellow> again for general help on skills.<c=/>");
		}
		else
		{
			i_skills.set_data("text_help", "\
<c=yellow>Press the letter attached to a skill to improve it.  You cannot \
improve a skill beyond its cap.\n\n\
Press <c=pink>?<c=yellow> and then a letter for skill description.<c=/>");
		}

		i_skills.draw(skillScreen);
	}
	else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
	{
// We picked a skill!
		int sk_num = ch;
		if (sk_num >= 'a' && sk_num <= 'z')
		{
			sk_num = sk_num - 'a' + 1;
		}
		else
		{
			sk_num = sk_num - 'Z' + 27;
		}
		Skill_type sk = Skill_type(sk_num);

		if (helpMode)
		{
			helpMode = false;
//			help_skill_desc(sk);
		}
		else
		{
			int cost = player->skills.improve_cost(sk);
			if (player->skills.maxed_out(sk) && player->has_trait(TRAIT_AUTODIDACT))
			{
				cost *= 2;
			}
			if (player->skills.maxed_out(sk) && !player->has_trait(TRAIT_AUTODIDACT))
			{
				showMessagePopup("That skill has hit its cap!");
			}
			else if (player->experience >= cost)
			{
				if (showQueryYesNo(Doryen::format(
						"Spend {} XP to increase {} to {}?", cost, skill_type_user_name(sk), player->skills.get_level(sk) + 1)))
				{
					player->experience -= cost;
					player->skills.increase_level(sk);
				}
			}
			else
			{
				showMessagePopup(Doryen::format(
						"You do not have enough XP! (You: {}; {} required)", player->experience, cost));
			}
		}

		i_skills.draw(skillScreen);

	} // End of "ch is a letter"

	return ScreenType::NONE;
}
