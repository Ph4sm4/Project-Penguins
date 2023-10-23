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

    void (*constructGrid)(struct GameGrid *obj);
    void (*printGridState)(const struct GameGrid *obj, const enum GameState phase);
    int (*getGridSize)(struct GameGrid *obj);
    void (*checkForBlockedPenguins)(struct GameGrid *gameGrid);
    bool (*isPointInBounds)(const struct GameGrid *gameGrid, const int x, const int y);
};

struct GameGrid createGameGridObject();

#endif