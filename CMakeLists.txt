CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

PROJECT(Cataclysm LANGUAGES CXX)

ADD_LIBRARY(Cuss
        cuss.cpp
        cuss_editor.cpp
        window.cpp
        glyph.cpp
        color.cpp
        stringfunc.cpp
        files.cpp
        )

TARGET_LINK_LIBRARIES(Cuss PRIVATE ncurses)

ADD_EXECUTABLE(Cataclysm
        attack.cpp
        field.cpp
        mission.cpp
        stringfunc.cpp
        biome.cpp
        files.cpp
        monster_ability.cpp
        terrain.cpp
        catacurse.cpp
        game.cpp
        monster.cpp
        time.cpp
        color.cpp
        geometry.cpp
        monster_spawn.cpp
        tool.cpp
        cuss.cpp
        globals.cpp
        monster_type.cpp
        trait.cpp
        glyph.cpp
        newcharacter.cpp
        var_string.cpp
        help.cpp
        pathfind.cpp
        window.cpp
        damage_set.cpp
        item.cpp
        player_activity.cpp
        worldmap.cpp
        dice.cpp
        item_type.cpp
        player.cpp
        worldmap_generate.cpp
        entity_ai.cpp
        keybind.cpp
        profession.cpp
        world_terrain.cpp
        entity.cpp
        main.cpp
        rng.cpp
        enum.cpp
        map.cpp
        skill.cpp
        explosion.cpp
        mapgen.cpp
        status_effect.cpp
        )

TARGET_LINK_LIBRARIES(Cataclysm PRIVATE Cuss)
TARGET_LINK_LIBRARIES(Cataclysm PRIVATE ncurses)

FILE(COPY data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)
FILE(COPY cuss/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cuss/)