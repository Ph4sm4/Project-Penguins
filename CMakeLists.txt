cmake_minimum_required(VERSION 3.10)
project(ProjectPenguins)

# Explicitly list the source files you want to include
set(SOURCES
    main.c
    GameGrid/Grid.c
    Player/Player.c
    GameSystem/GameSystem.c
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-g")


# Create an executable from the source files
add_executable(ProjectPenguins ${SOURCES})

# add_subdirectory(GameGrid)

# add_executable(main main.c)
# # We need hello.h and the hello library
# target_link_libraries(main
#     PRIVATE GameGrid)