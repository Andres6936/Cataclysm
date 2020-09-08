// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_OBSERVERMAP_HPP
#define CATACLYSM_OBSERVERMAP_HPP

#include <Cataclysm/Geometry/geometry.h>

namespace Cataclysm
{

	class ObserverMap
	{

	public:

		bool isEmpty(const Tripoint& _position) const noexcept;

	};

}

inline Cataclysm::ObserverMap observerMap {};

#endif //CATACLYSM_OBSERVERMAP_HPP
