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
	 * <br><br>
	 *
	 * <h3>Problem</h3>
	 *
	 * You’re making a game that has a bunch of different screens. Even in just
	 * a small game, you’ve probably got menu screens, a high score screen, a
	 * “normal play” screen, maybe a shop screen, and more. There are four
	 * separate issues here, but it turns out that they all are related enough
	 * to have a common solution:
	 * <br><br>
	 *
	 * 1. How do you manage the loading and unloading of the many screens,
	 * assuming you can’t keep the data/graphics all loaded at once (and with
	 * full screen background images, it’s not usually a good idea).
	 * <br><br>
	 *
	 * 2. How do you switch between screens? What is the easiest way to organize
	 * all of these different sections of game logic?
	 * <br><br>
	 *
	 * 3. How do you switch between control schemes? <br><br>
	 *
	 * 4. For ease of testing, how can you jump into a particular screen,
	 * assuming that a bunch of stuff has to be loaded for the scene to run
	 * correctly?
	 * <br><br>
	 *
	 * <h3>Early ideas</h3>
	 *
	 * At first I thought, “Let’s keep all the data and control logic in a
	 * single static class. We can divide up the game into states, one
	 * representing each different set of control logic, and do a switch
	 * statement to decide which data, game logic, etc. to use at any give
	 * time.”
	 * <br><br>
	 *
	 * The problem here was that it isn’t very scalable. It works fine for a
	 * very small number of simple screens/control schemes, but increasing the
	 * number or the complexity of the screens/control schemes and things get
	 * out of hand very quickly. Also, what constitutes a game state is pretty
	 * vague. Is it when the screen’s background has an obvious change? Is it
	 * each GUI? Are there only two states (just the play and menu)? Is it a
	 * new state when only the control logic changes, but nothing new has to
	 * be loaded?
	 * <br><br>
	 *
	 * This is basically how I started coding Cash Cow, and because I was
	 * answering all of those previous questions a different way each time,
	 * the control structure for the states grew more and more complex.
	 * Things were never consistent, which made adding and changes states
	 * very difficult. It also didn’t address issue number 4 at all, which
	 * made testing tedious because we had to navigate through menus to get to
	 * any particular part of the game. By the time I started coding Primate
	 * Panic, we had a much better idea.
	 * <br><br>
	 *
	 * <h3>Our Solution</h3>
	 *
	 * I call it “The Scene System.” Essentially, you divide the game up into
	 * scenes, where a scene is (usually) defined by a change in the background
	 * image. This means that the main menu screen is a scene, the map screen
	 * is a scene, the normal gameplay is a scene, etc. Why divide them up this
	 * way? Because in our current platform, background images take a noticeable
	 * amount of time to load (at least enough to cause the cursor to flinch).
	 * The idea is that if you only load a new background between scenes when
	 * the entire screen is black, the user won’t notice any delay from loading.
	 * <br><br>
	 *
	 * This means that each scene should have a natural way to transition in and
	 * transition out, and the unloading/loading takes place after the
	 * transition out of one scene and before the transition in of the next.
	 * In Primate Panic, you see the end of a scene when the screen fades to
	 * black. This is when the next scene is being loaded, but on most computers
	 * it is fast enough that you don’t notice any load time.
	 * <br><br>
	 *
	 * There is also a class that manages the scenes, and tells the current
	 * scene to update its game logic and redraw everything each frame.
	 * <br><br>
	 */
	class ScreenManager
	{

	private:

		/**
		 * This variable will always be true as long as the user does not want
		 * to exit of the application.
		 */
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

		/**
		 * Responsibility: Get the input of the current scene and change the
		 * scene if is need.
		 */
		void processInput();

		// Getters

		/**
		 * @return True if the user want to exit of the application, false otherwise.
		 */
		bool isRunning() const;

	};

}

#endif //CATACLYSM_SCREENMANAGER_HPP
