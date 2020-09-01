// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_NEWGAMESCREEN_HPP
#define CATACLYSM_NEWGAMESCREEN_HPP

#include <Cataclysm/Screen/IScreen.hpp>
#include <Cataclysm/Screen/NewGame/DoneScreen.hpp>
#include <Cataclysm/Screen/NewGame/StatsScreen.hpp>
#include <Cataclysm/Screen/NewGame/TraitsScreen.hpp>
#include <Cataclysm/Screen/NewGame/CancelScreen.hpp>
#include <Cataclysm/Screen/NewGame/ProfessionScreen.hpp>
#include <Cataclysm/Screen/NewGame/DescriptionScreen.hpp>
#include <Cataclysm/Screen/NewGame/Interface/INewGameScreen.hpp>

namespace Cataclysm
{

	class NewGameScreen : public IScreen
	{

	private:

		std::shared_ptr<INewGameScreen> doneScreen = std::make_shared<DoneScreen>();
		std::shared_ptr<INewGameScreen> statsScreen = std::make_shared<StatsScreen>();
		std::shared_ptr<INewGameScreen> traitsScreen = std::make_shared<TraitsScreen>();
		std::shared_ptr<INewGameScreen> cancelScreen = std::make_shared<CancelScreen>();
		std::shared_ptr<INewGameScreen> professionScreen = std::make_shared<ProfessionScreen>();
		std::shared_ptr<INewGameScreen> descriptionScreen = std::make_shared<DescriptionScreen>();

		std::shared_ptr<INewGameScreen> currentScreen {nullptr};

	public:

		NewGameScreen();

		void draw() override;

		void updated() override;

		ScreenType processInput() override;

	};

}

#endif //CATACLYSM_NEWGAMESCREEN_HPP
