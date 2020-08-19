// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ITEMACTION_HPP
#define CATACLYSM_ITEMACTION_HPP

// Item_action & Item_flag have their lookup and name functions defd in item.cpp
enum Item_action
{
	IACT_NULL = 0,
	IACT_WIELD,   // Wield as a wearpon
	IACT_WEAR,    // Wear - only applies for clothing
	IACT_DROP,    // Drop on the ground
	IACT_EAT,     // Eat - only applies for food
	IACT_APPLY,   // Apply as in a tool
	IACT_READ,    // Read - only applies for books
	IACT_UNLOAD,  // Applies for launchers and containers
	IACT_RELOAD,  // Only applies for launchers
	IACT_BUTCHER, // Cut into pieces of meat - only applies for corpses
	IACT_MAX
};

#endif //CATACLYSM_ITEMACTION_HPP
