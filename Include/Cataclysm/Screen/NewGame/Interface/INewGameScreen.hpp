// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_INEWGAMESCREEN_HPP
#define CATACLYSM_INEWGAMESCREEN_HPP

#include <Doryen/Doryen.hpp>
#include <Cuss/Interface.hpp>
#include <Cataclysm/Enum/trait.h>
#include <Cataclysm/Screen/IScreen.hpp>
#include <Cataclysm/Screen/NewGame/Enum/StatSelected.hpp>
#include <Cataclysm/Screen/NewGame/Enum/NewGameScreenType.hpp>

namespace Cataclysm
{

	class INewGameScreen : public IScreen
	{

	protected:

		inline static New_char_screen cur_screen = New_char_screen::NCS_STATS;

		inline static Stat_selected cur_stat = Stat_selected::STATSEL_STR;

		inline static Doryen::Console newGameConsole {80, 24};

		inline static cuss::interface i_newch;

		inline static std::vector<std::string> profession_list;

		/**
		 * We need to set up a list of traits which does NOT include the placeholder /
	 	 * marker "traits" like TRAIT_MAX_GOOD and TRAIT_MAX_NEUTRAL etc.
	 	 */
		inline static std::vector<Trait_id> selectable_traits;

		inline static std::vector<std::string> traits_list;

		inline static std::string filenameInterfaceCuss;

		inline static bool userChangedScreen = false;

		inline static int* stat_value = nullptr;

		inline static std::int32_t points = 4;

		inline static std::int32_t num_traits = 0;

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

		INewGameScreen();

		virtual void draw();

		virtual void updated();

		virtual ScreenType processInput();

	};

}

#endif //CATACLYSM_INEWGAMESCREEN_HPP
