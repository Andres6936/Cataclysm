// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_SOUNDMANAGER_HPP
#define CATACLYSM_SOUNDMANAGER_HPP

#include <string>
#include <Cataclysm/Geometry/geometry.h>
#include "Cataclysm/Mechanism/sound.h"

namespace Cataclysm
{

	class SoundManager
	{

	public:

		void makeSound(Sound snd, Tripoint pos);

		void makeSound(std::string desc, int volume, Point pos);

		void makeSound(std::string desc, int volume, Tripoint pos);

		void makeSound(std::string desc, int volume, int x, int y);

	};

}

inline Cataclysm::SoundManager soundManager {};

#endif //CATACLYSM_SOUNDMANAGER_HPP
