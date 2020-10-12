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

		std::shared_ptr<IScreen> menuScreen {nullptr};

		std::shared_ptr<IScreen> tourScreen {nullptr};

		std::shared_ptr<IScreen> playScreen {nullptr};

		std::shared_ptr<IScreen> worldScreen {nullptr};

		std::shared_ptr<IScreen> skillScreen {nullptr};

		std::shared_ptr<IScreen> actualScreen {nullptr};

		std::shared_ptr<IScreen> newGameScreen {nullptr};

		std::shared_ptr<IScreen> helpMenuScreen {nullptr};

		std::shared_ptr<IScreen> createWorldScreen {nullptr};

		std::shared_ptr<IScreen> showWorldmapScreen {nullptr};

		std::shared_ptr<IScreen> informationItemScreen {nullptr};

		std::shared_ptr<IScreen> inventorySingleSelectionScreen {nullptr};

		std::shared_ptr<IScreen> inventoryMultipleSelectionScreen {nullptr};

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
