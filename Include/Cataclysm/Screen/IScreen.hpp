// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SCREEN_HPP
#define CATACLYSM_SCREEN_HPP

#include <Doryen/Doryen.hpp>
#include <Cataclysm/Screen/Enum/ScreenType.hpp>

namespace Cataclysm
{

	class IScreen
	{

	protected:

		inline static Doryen::Console console {80, 24};

	public:

		virtual void draw() = 0;

		virtual void updated() = 0;

		virtual ScreenType processInput() = 0;

	};

}

#endif //CATACLYSM_SCREEN_HPP
