cmake_minimum_required(VERSION 3.10)
project(ProjectPenguins)


set(SOURCES
    main.c
    GameGrid/Grid.c
    Player/Player.c
    GameSystem/GameSystem.c
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-g")


add_executable(ProjectPenguins ${SOURCES})