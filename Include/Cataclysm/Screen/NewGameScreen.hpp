// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_NEWGAMESCREEN_HPP
#define CATACLYSM_NEWGAMESCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class NewGameScreen : public IScreen
	{

	private:

		enum class New_char_screen : std::uint8_t
		{
			NCS_CANCEL,
			NCS_STATS,
			NCS_TRAITS,
			NCS_PROFESSION,
			NCS_DESCRIPTION,
			NCS_DONE
		};

		enum class Stat_selected : std::uint8_t
		{
			STATSEL_STR,
			STATSEL_DEX,
			STATSEL_PER,
			STATSEL_INT
		};

		New_char_screen cur_screen = New_char_screen::NCS_STATS;

		Stat_selected cur_stat = Stat_selected::STATSEL_STR;

		Doryen::Console newGameConsole {80, 24};

		cuss::interface i_newch;

		std::vector<std::string> profession_list;

		/**
		 * We need to set up a list of traits which does NOT include the placeholder /
	 	 * marker "traits" like TRAIT_MAX_GOOD and TRAIT_MAX_NEUTRAL etc.
	 	 */
		std::vector<Trait_id> selectable_traits;

		std::vector<std::string> traits_list;

		bool userChangedScreen = false;

		int* stat_value = nullptr;

		std::int32_t points = 4;

		std::int32_t num_traits = 0;

		// Methods

		void prevScreen();

		void nextScreen();

		void prevStat();

		void nextStat();

		void verifyInvariantPlayer();

		std::string getStatDescription(Stat_selected stat);

		std::vector<std::string> getTraitList();

		std::vector<std::string> getProfessionList();

	public:

		NewGameScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_NEWGAMESCREEN_HPP
