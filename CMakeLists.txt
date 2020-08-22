CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

PROJECT(Cataclysm LANGUAGES CXX)

# Content Package Manager
INCLUDE(CMakeModules/CPM.cmake)

CPMAddPackage(
        NAME Doryen
        GITHUB_REPOSITORY Andres6936/Doryen
        VERSION 2020.0820
)

ADD_LIBRARY(Cuss
        Source/Cuss/Enum/ActionID.cpp
        Source/Cuss/String/Utility.cpp
        Source/Cuss/List.cpp
        Source/Cuss/Menu.cpp
        Source/Cuss/Number.cpp
        Source/Cuss/Binding.cpp
        Source/Cuss/Drawing.cpp
        Source/Cuss/Element.cpp
        Source/Cuss/TextBox.cpp
        Source/Cuss/TextEntry.cpp
        Source/Cuss/Interface.cpp
        Source/cuss_editor.cpp
        Source/window.cpp
        Source/glyph.cpp
        Source/color.cpp
        Source/stringfunc.cpp
        Source/files.cpp
        )

# Compile Cataclysm with Support to C++17
SET_TARGET_PROPERTIES(Cuss PROPERTIES CXX_STANDARD 17)
# Find the headers in the directory Cataclysm without path to Include
TARGET_INCLUDE_DIRECTORIES(Cuss PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/>
        $<INSTALL_INTERFACE:/Include/>)
TARGET_LINK_LIBRARIES(Cuss PRIVATE Doryen::Framework)
TARGET_LINK_LIBRARIES(Cuss PRIVATE ncurses)

ADD_EXECUTABLE(Cataclysm
        Source/Attack/Verbs.cpp
        Source/Attack/attack.cpp
        Source/Attack/Ranged.cpp
        Source/Entity/Plan.cpp
        Source/Entity/Pool.cpp
        Source/Entity/Stats.cpp
        Source/Entity/entity.cpp
        Source/Entity/profession.cpp
        Source/Entity/Monster/Type.cpp
        Source/Entity/Monster/Size.cpp
        Source/Entity/Monster/Genus.cpp
        Source/Entity/Monster/Ability.cpp
        Source/Entity/Monster/monster.cpp
        Source/Entity/Monster/monster_spawn.cpp
        Source/Entity/Monster/AbilityFields.cpp
        Source/Entity/Monster/AbilitySignal.cpp
        Source/Entity/Monster/AbilityStatus.cpp
        Source/Entity/Monster/AbilitySummon.cpp
        Source/Entity/Monster/AbilityTerrain.cpp
        Source/Entity/Monster/AbilityTeleport.cpp
        Source/Entity/Monster/Enum/AbilityType.cpp
        Source/Entity/Player/player.cpp
        Source/Entity/Player/player_activity.cpp
        Source/Entity/Player/Enum/ActivityType.cpp
        Source/Enum/BodyPart.cpp
        Source/Enum/StatType.cpp
        Source/Enum/ItemFlag.cpp
        Source/Enum/SenseType.cpp
        Source/Enum/DamageType.cpp
        Source/Enum/MathOperator.cpp
        Source/Item/Enum/Class.cpp
        Source/Item/Enum/BookGenre.cpp
        Source/Item/item.cpp
        Source/Item/TypeAmmo.cpp
        Source/Item/TypeBook.cpp
        Source/Item/TypeFood.cpp
        Source/Item/TypeTool.cpp
        Source/Item/item_type.cpp
        Source/Item/TypeCorpse.cpp
        Source/Item/TypeClothing.cpp
        Source/Item/TypeLauncher.cpp
        Source/Item/TypeContainer.cpp
        Source/Screen/Debugger.cpp
        Source/World/Biome/Enum/BiomeFlag.cpp
        Source/World/Biome/biome.cpp
        Source/World/Biome/VariableMonsterGenus.cpp
        Source/World/Tile.cpp
        Source/World/worldmap.cpp
        Source/World/world_terrain.cpp
        Source/World/VariableTerrain.cpp
        Source/World/Enum/SpreadType.cpp
        Source/World/Enum/TerrainFlag.cpp
        Source/World/Terrain/Bonus.cpp
        Source/World/Terrain/Smash.cpp
        Source/World/Terrain/terrain.cpp
        Source/World/Terrain/FlagBonus.cpp
        Source/World/Terrain/SignalHandler.cpp
        Source/field.cpp
        Source/mission.cpp
        Source/stringfunc.cpp
        Source/files.cpp
        Source/game.cpp
        Source/time.cpp
        Source/color.cpp
        Source/geometry.cpp
        Source/tool.cpp
        Source/globals.cpp
        Source/trait.cpp
        Source/glyph.cpp
        Source/newcharacter.cpp
        Source/var_string.cpp
        Source/help.cpp
        Source/pathfind.cpp
        Source/window.cpp
        Source/damage_set.cpp
        Source/dice.cpp
        Source/worldmap_generate.cpp
        Source/entity_ai.cpp
        Source/keybind.cpp
        Source/main.cpp
        Source/rng.cpp
        Source/enum.cpp
        Source/map.cpp
        Source/skill.cpp
        Source/explosion.cpp
        Source/mapgen.cpp
        Source/status_effect.cpp
        )

# Compile Cataclysm with Support to C++17
SET_TARGET_PROPERTIES(Cataclysm PROPERTIES CXX_STANDARD 17)
# Find the headers in the directory Cataclysm without path to Include
TARGET_INCLUDE_DIRECTORIES(Cataclysm PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/>
        $<INSTALL_INTERFACE:/Include/>)
# Link the libraries needed
TARGET_LINK_LIBRARIES(Cataclysm PRIVATE Doryen::Framework)
TARGET_LINK_LIBRARIES(Cataclysm PRIVATE Cuss)
TARGET_LINK_LIBRARIES(Cataclysm PRIVATE ncurses)

FILE(COPY data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)
FILE(COPY cuss/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cuss/)