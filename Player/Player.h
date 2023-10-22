#ifndef PLAYER_H
#define PLAYER_H

struct Player
{
    char *name;
    int collectedFishes;

    int blockedPenguins;
};

struct Player createPlayerObject();

#endif