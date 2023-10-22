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
void placeAPenguin(struct GameGrid *gameGrid, struct GridPoint *point, struct GameSystem *game);
void printfError(char *message);
bool isPointInBounds(const struct GameGrid *gameGrid, const int x, const int y);
struct Player *getCurrentPlayer(struct GameSystem *game);

// =========================================

struct GameSystem
createGameSystemObject()
{
    struct GameSystem obj;
    // setting all of the objects
    obj.GameGrid = createGameGridObject();
    obj.phase = (enum GameState)PlacingPhase;

    obj.player_1 = createPlayerObject();
    obj.player_1.index = 0;

    obj.player_2 = createPlayerObject();
    obj.player_2.index = 1;

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
    printfOrange("%s's (P1)", game->player_1.name);
    printf(" points: %d\n", game->player_1.collectedFishes);

    printfGreen("%s's (P2)", game->player_2.name);
    printf(" points: %d\n", game->player_2.collectedFishes);

    game->GameGrid.printGridState(&game->GameGrid, game->phase);
}

struct Player *getCurrentPlayer(struct GameSystem *game)
{
    return (!game->playerTurn ? &game->player_1 : &game->player_2);
}

void askForAction(struct GameSystem *game)
{
    switch (game->phase)
    {
    case (enum GameState)PlacingPhase:
    {
        int x, y;

        printf("\n\n%s's turn to place a penguin (x,y): ", getCurrentPlayer(game)->name);
        scanf("%d %d", &x, &y);

        // 0 based x's and y's
        x--;
        y--;

        static int placedPenguins = 0;

        if (!isPointInBounds(&game->GameGrid, x, y))
        {
            printfError("The following coordinates exceed the grid size!");
            return;
        }
        struct GridPoint *p = &game->GameGrid.grid[x][y];

        if (p->numberOfFishes != 1)
        {
            printfError("You can only place penguins on unocuppied tiles with exactly 1 fish on it");
        }
        else if (p->owner != NULL)
        {
            printfError("This tile is occupied, choose a different one");
        }
        else
        {
            placeAPenguin(&game->GameGrid, p, game);
            placedPenguins++;

            getCurrentPlayer(game)->collectedFishes++;
            game->playerTurn = game->playerTurn ^ 1;

            if (placedPenguins == game->numberOfPenguins * 2)
            {
                game->phase = (enum GameState)MovementPhase;
            }
        }
        break;
    }
    case (enum GameState)MovementPhase:
    {
        int penguinX, penguinY;
        int toX, toY; // move to
        printf("\n\n%s's turn to move. First choose a penguin (x, y): ", getCurrentPlayer(game)->name);
        scanf("%d %d", &penguinX, &penguinY);

        penguinX--;
        penguinY--;

        if (!isPointInBounds(&game->GameGrid, penguinX, penguinY))
        {
            printfError("The following coordinates exceed the grid size!");
            return;
        }
        struct GridPoint *p = &game->GameGrid.grid[penguinX][penguinY];

        if (p->owner == NULL)
        {
            printfError("This is not a penguin");
        }
        else if (p->owner != getCurrentPlayer(game))
        {
            char t[] = "This penguin belongs to ";
            strcat(t, (game->playerTurn == 0 ? game->player_2.name : game->player_1.name));
            printfError(t);
        }
        else
        {
            p->selected = true;
        }

        break;
    }
    }
}

bool isPointInBounds(const struct GameGrid *gameGrid, const int x, const int y)
{
    return x >= 0 && x < gameGrid->rows && y >= 0 && y < gameGrid->cols;
}

void printfError(char *message)
{
    printfRed("%s", message);
    getchar();
    getchar();
}

void placeAPenguin(struct GameGrid *gameGrid, struct GridPoint *point, struct GameSystem *game)
{
    point->owner = getCurrentPlayer(game);
    point->removed = true;
    strcpy(point->label, game->playerTurn == 0 ? "P1" : "P2");
}

void moveAPenguin(struct GameGrid *gameGrid, const int xFrom, const int yFrom, const int xTo, const int yTo)
{
}
