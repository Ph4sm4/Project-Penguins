#ifndef GRID_H
#define GRID_H

#include "GridPoint.h"
#include "../Enums/GameState.h"

struct GameGrid
{
    int rows;
    int cols;
    struct GridPoint **grid;

    float *noiseSeed2D;
    float *perlinNoise2D;

    // Function to construct the grid based on specified rows and columns
    void (*constructGrid)(struct GameGrid *obj);

    // Function to print the current state of the grid based on the game phase
    void (*printGridState)(const struct GameGrid *obj, const enum GameState phase);

    // Function to get the total size of the grid (rows * columns)
    int (*getGridSize)(struct GameGrid *obj);

    // Function to check for blocked penguins on the grid and update the blocked penguins count
    void (*checkForBlockedPenguins)(struct GameGrid *gameGrid);

    // Function to check if a point is within the bounds of the grid
    bool (*isPointInBounds)(const struct GameGrid *gameGrid, const int x, const int y);
};

struct GameGrid createGameGridObject();

#endif