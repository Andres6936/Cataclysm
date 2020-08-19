CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

PROJECT(Cataclysm LANGUAGES CXX)

ADD_LIBRARY(Cuss
        Source/cuss.cpp
        Source/cuss_editor.cpp
        Source/window.cpp
        Source/glyph.cpp
        Source/color.cpp
        Source/stringfunc.cpp
        Source/files.cpp
        )

TARGET_LINK_LIBRARIES(Cuss PRIVATE ncurses)

# Content Package Manager
INCLUDE(CMakeModules/CPM.cmake)

CPMAddPackage(
        NAME Doryen
        GITHUB_REPOSITORY Andres6936/Doryen
        VERSION 2020.0819
)

ADD_EXECUTABLE(Cataclysm
        Source/attack.cpp
        Source/field.cpp
        Source/mission.cpp
        Source/stringfunc.cpp
        Source/biome.cpp
        Source/files.cpp
        Source/monster_ability.cpp
        Source/terrain.cpp
        Source/catacurse.cpp
        Source/game.cpp
        Source/monster.cpp
        Source/time.cpp
        Source/color.cpp
        Source/geometry.cpp
        Source/monster_spawn.cpp
        Source/tool.cpp
        Source/cuss.cpp
        Source/globals.cpp
        Source/monster_type.cpp
        Source/trait.cpp
        Source/glyph.cpp
        Source/newcharacter.cpp
        Source/var_string.cpp
        Source/help.cpp
        Source/pathfind.cpp
        Source/window.cpp
        Source/damage_set.cpp
        Source/item.cpp
        Source/player_activity.cpp
        Source/worldmap.cpp
        Source/dice.cpp
        Source/item_type.cpp
        Source/player.cpp
        Source/worldmap_generate.cpp
        Source/entity_ai.cpp
        Source/keybind.cpp
        Source/profession.cpp
        Source/world_terrain.cpp
        Source/entity.cpp
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