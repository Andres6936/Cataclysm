// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_TARGETSELECTORMANAGER_HPP
#define CATACLYSM_TARGETSELECTORMANAGER_HPP

#include <vector>
#include <Cataclysm/Geometry/geometry.h>

namespace Cataclysm
{

	class TargetSelectorManager
	{

	private:

		// Variables

		std::int32_t last_target {-1};

		// Methods

		std::int32_t getLevelLight();

		std::vector<Tripoint> pathSelector(int startx = -1, int starty = -1, int range = -1, bool target_entities = false, bool show_path = true);

	public:

		Tripoint select(int startx = -1, int starty = -1, int range = -1, bool target_entites = false, bool show_path = false);

	};

}

inline Cataclysm::TargetSelectorManager targetSelectorManager {};

#endif //CATACLYSM_TARGETSELECTORMANAGER_HPP
