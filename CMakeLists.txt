cmake_minimum_required(VERSION 3.30)
project(HEX-GAME)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.1.4
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SFML fmt)

set_source_files_properties(Main.cpp Board.cpp PROPERTIES
        OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Theme.h
)


add_executable(HEX-GAME
        Tile.cpp
        Board.cpp
        Main.cpp
        BotLogic.cpp
)
target_link_libraries(HEX-GAME  SFML::Graphics fmt::fmt)

