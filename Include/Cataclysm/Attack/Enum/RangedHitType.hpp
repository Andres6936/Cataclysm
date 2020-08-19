// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_RANGEDHITTYPE_HPP
#define CATACLYSM_RANGEDHITTYPE_HPP

enum Ranged_hit_type
{
	RANGED_HIT_NULL = 0,
	RANGED_HIT_GRAZE,     // Damage is between 0 and full
	RANGED_HIT_NORMAL,    // Damage is between 80% and 100%
	RANGED_HIT_CRITICAL,  // Damage is between 100% and 150%
	RANGED_HIT_HEADSHOT,  // Damage is between 300% and 500%
	RANGED_HIT_MAX
};

#endif //CATACLYSM_RANGEDHITTYPE_HPP
