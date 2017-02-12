cmake_minimum_required(VERSION 3.6)
project(Labyrinth)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        about.png
        aboutProgramm.png
        base.hpp
        exitProgramm.png
        game.ogg
        intro.ogg
        Level1.png
        Level2.png
        Level3.png
        Level4.png
        main.cpp
        Mario_Theme.ogg
        metka.png
        metka1.png
        "Labyrinth-Info.plist"
        newGame.png
        ResourcePath.hpp
        ResourcePath.mm
        sansation.ttf)

add_executable(Labyrinth ${SOURCE_FILES})