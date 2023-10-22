#include "Player.h"
#include "stdlib.h"

struct Player createPlayerObject()
{
    struct Player obj;
    obj.collectedFishes = 0;
    obj.name = (char *)malloc(100);

    return obj;
}