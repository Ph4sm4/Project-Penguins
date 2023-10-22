#ifndef GRID_POINT_H
#define GRID_POINT_H

#include <stdbool.h>
#include "../Enums/TileOwner.h"

struct GridPoint
{
    int x;
    int y;
    char *label;
    bool removed;
    int numberOfFishes;

    enum TileOwner owner;
};

#endif