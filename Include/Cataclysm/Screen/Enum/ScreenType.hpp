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
		PAUSE,
		HELP_MENU,
		CREATE_WORLD,
		CREATE_PLAYER,
	};

}

#endif //CATACLYSM_SCREENTYPE_HPP
