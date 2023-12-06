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

    // Function to set up the game, including grid size, player names, and initial grid state
    void (*setup)(struct GameSystem *game);

    // Function to display information about the current points
    // during finish phase it also summarizes the game
    void (*displayUI)(struct GameSystem *game);

    // Function to ask for player action based on the current game phase (Placing, Movement, Finish)
    void (*askForAction)(struct GameSystem *game);
};

// creates the object and sets all default values including references to functions
struct GameSystem createGameSystemObject();

#endif