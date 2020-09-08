// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_PROJECTILE_HPP
#define CATACLYSM_PROJECTILE_HPP

#include <Cataclysm/Entity/entity.h>

namespace Cataclysm
{

	class Projectile
	{

	public:

		void launch(Ranged_attack attack, Tripoint origin, Tripoint target);

		void launch(Entity* shooter, Ranged_attack attack, Tripoint origin, Tripoint target);

		void launch(Entity* shooter, Item it, Ranged_attack attack, Tripoint origin, Tripoint target);

	};

}

inline Cataclysm::Projectile projectile {};

#endif //CATACLYSM_PROJECTILE_HPP
