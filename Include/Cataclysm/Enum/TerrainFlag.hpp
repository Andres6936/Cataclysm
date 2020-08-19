// Joan Andrés (@Andres6936) Github.

#pragma once

// Terrain_flag has its lookup and name functions in terrain.cpp
enum Terrain_flag
{
	TF_NULL,
	TF_OPAQUE,        // "opaque" - Blocks sight
	TF_FLOOR,         // "floor" - Displays items on top of it
	TF_MUTABLE,       // "mutable" - May be changed by adjacency maps
	TF_STAIRS_UP,     // "stairs_up" - Can be climbed to gain a Z-level
	TF_STAIRS_DOWN,   // "stairs_down" - Can be climbed to lose a Z-level
	TF_OPEN_SPACE,    // "open_space" - Air.  Shows Z-level below.
	TF_WATER,         // "water" - Swimmable.  Puts out fire.
	TF_FLAMMABLE,     // "flammable" - Consumed by fire.
	TF_CONTAINER,     // "container" - Can hold items despite move_cost of 0
	TF_PLURAL,        // "plural" - Indefinite article is "some" (instead of "a")
	TF_INDOORS,       // "indoors" - You can open locked doors from this tile.
	TF_SEALED,        // "sealed" - You can't pick up or see items here.
	TF_EXPLOSIVE,     // "explosive" - Explodes in fire.
	TF_NO_ITEMS,      // "no_items" - Items can't be dropped here (they fall off)
	TF_MAX
};

Terrain_flag lookup_terrain_flag(std::string name);

std::string terrain_flag_name(Terrain_flag flag);