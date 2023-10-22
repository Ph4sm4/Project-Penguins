#include "GameSystem.h"
#include "stdio.h"
#include "string.h"
#include "../GameGrid/DisplayManager.h"

#define welcomeLine() printf("\n---- PROJECT \"PENGUINS\" ----\n\n");

// =========================================
// available public functions:

void setup(struct GameSystem *game);
void displayUI(struct GameSystem *game);
void askForAction(struct GameSystem *game);

// private functions:
void moveAPenguin(struct GameGrid *gameGrid, const int xFrom, const int yFrom, const int xTo, const int yTo);
void placeAPenguin(struct GameGrid *gameGrid, const int x, const int y, const bool playerTurn);
void printfError(char *message);

// =========================================

struct GameSystem
createGameSystemObject()
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
    obj.askForAction = &askForAction;

    obj.numberOfPenguins = 2;
    obj.playerTurn = 0;

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

void askForAction(struct GameSystem *game)
{
    int x, y;
    switch (game->phase)
    {
    case (enum GameState)PlacingPhase:
    {
        printf("\n\n%s's turn to place a penguin (x,y): ", (!game->playerTurn ? game->player_1.name : game->player_2.name));
        scanf("%d %d", &x, &y);

        // 0 based x's and y's
        x--;
        y--;

        static int placedPenguins = 0;

        if (!(x >= 0 && x < game->GameGrid.rows) || !(y >= 0 && y < game->GameGrid.cols))
        {
            printfError("The following coordinates exceed the grid size!");
        }
        else if (game->GameGrid.grid[x][y].numberOfFishes != 1)
        {
            printfError("You can only place penguins on unocuppied tiles with exactly 1 fish on it");
        }
        else if (game->GameGrid.grid[x][y].owner != (enum TileOwner)Unset)
        {
            printfError("This tile is occupied, choose a different one");
        }
        else
        {
            placeAPenguin(&game->GameGrid, x, y, game->playerTurn);
            placedPenguins++;

            game->playerTurn = game->playerTurn ^ 1;

            if (placedPenguins == game->numberOfPenguins * 2)
            {
                game->phase = (enum GameState)MovementPhase;
            }

            break;
        }
    case (enum GameState)MovementPhase:
    {
        break;
    }
    }
    }
}

void printfError(char *message)
{
    printfRed("%s", message);
    getchar();
    getchar();
}

void placeAPenguin(struct GameGrid *gameGrid, const int x, const int y, const bool playerTurn)
{
    struct GridPoint *p = &gameGrid->grid[x][y];

    p->owner = (enum TileOwner)(!playerTurn ? Player1 : Player2);
    p->removed = true;
    strcpy(p->label, !playerTurn ? "P1" : "P2");
}

void moveAPenguin(struct GameGrid *gameGrid, const int xFrom, const int yFrom, const int xTo, const int yTo)
{
}
