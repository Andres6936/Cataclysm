// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SCREEN_HPP
#define CATACLYSM_SCREEN_HPP

#include <Doryen/Doryen.hpp>
#include <Cataclysm/window.h>
#include <Cuss/Interface.hpp>
#include <Cataclysm/Screen/Enum/ScreenType.hpp>

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

	public:

		virtual void draw() = 0;

		virtual void updated() = 0;

		virtual ScreenType processInput() = 0;

	};

}

#endif //CATACLYSM_SCREEN_HPP
