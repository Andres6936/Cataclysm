// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SCREENMANAGER_HPP
#define CATACLYSM_SCREENMANAGER_HPP

#include <memory>

#include <Cataclysm/Screen/PlayScreen.hpp>
#include <Cataclysm/Screen/IScreen.hpp>

namespace Cataclysm
{

	/**
	 * The scene manager keeps track of the scenes in a game, allowing to switch
	 * between them. At it's basic, it provides a centralized place to load and
	 * unload the scenes, keeping track of which one is loaded and handle
	 * unloading that scene when a new one is loaded.
	 */
	class ScreenManager
	{

	private:

		bool running = true;

		std::shared_ptr<IScreen> menuScreen {nullptr};

		std::shared_ptr<IScreen> tourScreen {nullptr};

		std::shared_ptr<IScreen> playScreen {nullptr};

		std::shared_ptr<IScreen> worldScreen {nullptr};

		std::shared_ptr<IScreen> skillScreen {nullptr};

		/**
		 * A point to the current scene that is rendering.
		 */
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

		/**
		 * Initialize the first scene and makes that {actualScreen} to point to
		 * this first scene.
		 *
		 * Postcondition: The first scene has been initialized.
		 */
		ScreenManager();

		// Methods

		/**
		 * Responsibility: Call to method draw of the current scene.
		 */
		void draw();

		/**
		 * Responsibility: Call to method updated of the current the scene.
		 */
		void updated();

		void processInput();

		// Getters

		bool isRunning() const;

	};

}

#endif //CATACLYSM_SCREENMANAGER_HPP
