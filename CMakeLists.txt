CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

PROJECT(Cataclysm LANGUAGES CXX)

# Content Package Manager
INCLUDE(CMakeModules/CPM.cmake)

CPMAddPackage(
        NAME Doryen
        GITHUB_REPOSITORY Andres6936/Doryen
        VERSION 2020.0901
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
        Source/Graphics/glyph.cpp
        Source/Graphics/color.cpp
        Source/window.cpp
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
        Source/Entity/Player/StateInventory.cpp
        Source/Entity/Player/player_activity.cpp
        Source/Entity/Player/Enum/ActivityType.cpp
        Source/Geometry/geometry.cpp
        Source/Enum/trait.cpp
        Source/Enum/BodyPart.cpp
        Source/Enum/StatType.cpp
        Source/Enum/ItemFlag.cpp
        Source/Enum/SenseType.cpp
        Source/Enum/DamageType.cpp
        Source/Enum/MathOperator.cpp
        Source/Graphics/color.cpp
        Source/Graphics/glyph.cpp
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
        Source/Random/rng.cpp
        Source/Random/dice.cpp
        Source/Screen/IScreen.cpp
        Source/Screen/Debugger.cpp
        Source/Screen/MenuScreen.cpp
        Source/Screen/PlayScreen.cpp
        Source/Screen/TourScreen.cpp
        Source/Screen/WorldScreen.cpp
        Source/Screen/SkillScreen.cpp
        Source/Screen/ScreenManager.cpp
        Source/Screen/NewGameScreen.cpp
        Source/Screen/HelpMenuScreen.cpp
        Source/Screen/InventorySingleSelectionScreen.cpp
        Source/Screen/CreateWorldScreen.cpp
        Source/Screen/NewGame/DoneScreen.cpp
        Source/Screen/NewGame/StatsScreen.cpp
        Source/Screen/NewGame/CancelScreen.cpp
        Source/Screen/NewGame/TraitsScreen.cpp
        Source/Screen/NewGame/ProfessionScreen.cpp
        Source/Screen/NewGame/DescriptionScreen.cpp
        Source/Screen/NewGame/Interface/INewGameScreen.cpp
        Source/World/Biome/Enum/BiomeFlag.cpp
        Source/World/Biome/biome.cpp
        Source/World/Biome/VariableMonsterGenus.cpp
        Source/World/Tile.cpp
        Source/World/worldmap.cpp
        Source/World/world_terrain.cpp
        Source/World/VariableTerrain.cpp
        Source/World/worldmap_generate.cpp
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
        Source/time.cpp
        Source/tool.cpp
        Source/globals.cpp
        Source/newcharacter.cpp
        Source/var_string.cpp
        Source/pathfind.cpp
        Source/window.cpp
        Source/damage_set.cpp
        Source/entity_ai.cpp
        Source/keybind.cpp
        Source/main.cpp
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

FILE(COPY Fonts/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Fonts/)
FILE(COPY data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)
FILE(COPY cuss/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cuss/)
