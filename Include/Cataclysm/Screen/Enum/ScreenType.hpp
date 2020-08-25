// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SCREENTYPE_HPP
#define CATACLYSM_SCREENTYPE_HPP

namespace Cataclysm
{

	enum class ScreenType : unsigned char
	{
		NONE,
		QUIT,
		MENU,
		PLAY,
		TOUR,
		PAUSE,
		WORLD,
		NEW_GAME,
		HELP_MENU,
		CREATE_WORLD,
	};

}

#endif //CATACLYSM_SCREENTYPE_HPP
