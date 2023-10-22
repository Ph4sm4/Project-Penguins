#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include "../GameGrid/Grid.h"
#include "../Player/Player.h"
#include "stdbool.h"
#include "../Enums/GameState.h"

struct GameSystem
{
    struct GameGrid GameGrid;
    struct Player player_1;
    struct Player player_2;

    bool playerTurn; // 0 - player1, 1 - player2

    int numberOfPenguins;

    enum GameState phase;

    void (*setup)(struct GameSystem *game);
    void (*displayUI)(struct GameSystem *game);
    void (*askForAction)(struct GameSystem *game);
};

struct GameSystem createGameSystemObject();

#endif