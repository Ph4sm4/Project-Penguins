#ifndef PLAYER_H
#define PLAYER_H

struct Player
{
    char *name;
    int collectedFishes;
};

struct Player createPlayerObject();

#endif