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
    void (*cleanupGrid)(struct GameGrid *obj);
    int (*getGridSize)(struct GameGrid *obj);
};

struct GameGrid createGameGridObject();

#endif