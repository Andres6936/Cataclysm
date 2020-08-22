// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SCREENMANAGER_HPP
#define CATACLYSM_SCREENMANAGER_HPP

#include <memory>

#include <Cataclysm/game.h>
#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class ScreenManager
	{

	private:

		bool running = true;

		std::shared_ptr<IScreen> menuScreen {};

		std::shared_ptr<IScreen> actualScreen {};

		void nextScene(ScreenType _next);

	public:

		// Construct

		ScreenManager();

		// Methods

		void draw();

		void updated();

		void processInput();

		// Getters

		bool isRunning() const;

	};

}

#endif //CATACLYSM_SCREENMANAGER_HPP
