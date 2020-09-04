// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SCREEN_HPP
#define CATACLYSM_SCREEN_HPP

#include <Cataclysm/map.h>
#include <Doryen/Doryen.hpp>
#include <Cataclysm/window.h>
#include <Cuss/Interface.hpp>
#include <Cataclysm/World/worldmap.h>
#include <Cataclysm/Screen/Enum/ScreenType.hpp>
#include <Cataclysm/Entity/Player/player.h>
#include <Cataclysm/Entity/Pool.hpp>

namespace Cataclysm
{

	class IScreen
	{

	protected:

		inline static Doryen::Console console {80, 24};

		inline static std::shared_ptr<Window> w_map {nullptr};

		inline static std::shared_ptr<Window> w_hud {nullptr};

		inline static std::vector<std::string> worldmap_names;

		inline static cuss::interface i_hud {};

		inline static std::int32_t last_target {-1};

		inline static std::int32_t new_messages {0};

		/**
		 * -1 Not Select World
		 */
		inline static std::int32_t indexWorldSelect {-1};

		inline static bool game_over {false};

		// Methods

		bool showQueryYesNo(std::string_view _text);

		void showDebugMessage(std::string_view _text);

		void showMessagePopup(std::string_view _text);

	public:

		inline static std::unique_ptr<Worldmap> worldmap {nullptr};

		// Note that player should always == &(entities[0])
		inline static std::unique_ptr<Player> player {nullptr};

		inline static std::unique_ptr<Map> map {nullptr};

		inline static Entity_pool entities {};

		inline static Time time {};

		// Virtual Methods

		virtual void draw() = 0;

		virtual void updated() = 0;

		virtual ScreenType processInput() = 0;

	};

}

#endif //CATACLYSM_SCREEN_HPP
