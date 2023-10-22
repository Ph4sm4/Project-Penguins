#include "GameSystem.h"
#include "stdio.h"

#define welcomeLine() printf("\n---- PROJECT \"PENGUINS\" ----\n\n");

// =========================================
// available functions:

void setup(struct GameSystem *game);
void displayUI(struct GameSystem *game);

// =========================================

struct GameSystem createGameSystemObject()
{
    struct GameSystem obj;
    // setting all of the objects
    obj.GameGrid = createGameGridObject();
    obj.phase = (enum GameState)PlacingPhase;

    obj.player_1 = createPlayerObject();
    obj.player_2 = createPlayerObject();

    // setting function references
    obj.setup = &setup;
    obj.displayUI = &displayUI;

    obj.numberOfPenguins = 2;

    return obj;
}

void setup(struct GameSystem *game)
{
    welcomeLine();
    printf("Enter the number of rows: ");
    scanf("%d", &game->GameGrid.rows);
    printf("Enter the number of columns: ");
    scanf("%d", &game->GameGrid.cols);

    printf("\n ------- \n\n");

    char name1[100], name2[100];
    printf("Enter player's 1 nickname: ");
    scanf("%99s", game->player_1.name);
    printf("Enter player's 2 nickname: ");
    scanf("%99s", game->player_2.name);

    game->GameGrid.constructGrid(&game->GameGrid);
}

void displayUI(struct GameSystem *game)
{
    welcomeLine();
    printf("%s's points: %d\n", game->player_1.name, game->player_1.collectedFishes);
    printf("%s's points: %d\n", game->player_2.name, game->player_2.collectedFishes);

    game->GameGrid.printGridState(&game->GameGrid, game->phase);
}