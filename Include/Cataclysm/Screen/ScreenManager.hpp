// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SCREENMANAGER_HPP
#define CATACLYSM_SCREENMANAGER_HPP

#include <memory>

#include <Cataclysm/Screen/PlayScreen.hpp>
#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	class ScreenManager
	{

	private:

		bool running = true;

		std::shared_ptr<IScreen> menuScreen {};

		std::shared_ptr<IScreen> tourScreen {};

		std::shared_ptr<IScreen> playScreen {};

		std::shared_ptr<IScreen> worldScreen {};

		std::shared_ptr<IScreen> actualScreen {};

		std::shared_ptr<IScreen> helpMenuScreen {};

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
