// Joan Andrés (@Andres6936) Github.

#ifndef CATACLYSM_ITEMFLAG_HPP
#define CATACLYSM_ITEMFLAG_HPP

enum Item_flag
{
	ITEM_FLAG_NULL = 0,
	ITEM_FLAG_LIQUID,     // "liquid" - Puts out fires, needs a container
	ITEM_FLAG_FLAMMABLE,  // "flammable" - Consumed by fires
	ITEM_FLAG_PLURAL,     // "plural" - indefinite article is "some," not "a"
	ITEM_FLAG_CONSTANT,   // "constant_volume_weight" - doesn't use usual food v/w
	ITEM_FLAG_RELOAD_SINGLE,  // "reload_single" - reloading loads a single round
	ITEM_FLAG_OPEN_END,   // "open_end" - for containers.  Can hold 5x limit.
	ITEM_FLAG_GLASSES,    // Prevent short-sightedness
	ITEM_FLAG_RELOAD_STR, // "reload_strength" - Reduce reload_ap by Str * 30
	ITEM_FLAG_MAX
};

#endif //CATACLYSM_ITEMFLAG_HPP
