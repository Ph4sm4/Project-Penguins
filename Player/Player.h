#ifndef PLAYER_H
#define PLAYER_H

struct Player
{
    char *name;
    int index;
    int collectedFishes;
};

struct Player createPlayerObject();

#endif