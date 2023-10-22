#include "GameSystem.h"
#include "stdio.h"
#include "string.h"
#include "../GameGrid/DisplayManager.h"
#include "Windows.h"

#define welcomeLine() printf("\n---- PROJECT \"PENGUINS\" ----\n\n");

// =========================================
// available public functions:

void setup(struct GameSystem *game);
void displayUI(struct GameSystem *game);
void askForAction(struct GameSystem *game);

// private functions:
void moveAPenguin(struct GameSystem *game, struct GridPoint *initialPoint, struct GridPoint *destination);
void placeAPenguin(struct GameGrid *gameGrid, struct GridPoint *point, struct GameSystem *game);
void printfError(char *message);
bool isPointInBounds(const struct GameGrid *gameGrid, const int x, const int y);
struct Player *getCurrentPlayer(struct GameSystem *game);
struct GridPoint *getValidatedChoicePoint(struct GameSystem *game, const int x, const int y);
struct GridPoint *getValidatedMovePoint(struct GameSystem *game, struct GridPoint *initialPoint, const int x, const int y);
void checkForBlockedPenguins(struct GameGrid *gameGrid);
bool canMoveToPoint(struct GameGrid *gameGrid, const int x, const int y);

// =========================================

struct GameSystem
createGameSystemObject()
{
    struct GameSystem obj;
    // setting all of the objects
    obj.GameGrid = createGameGridObject();
    obj.phase = (enum GameState)PlacingPhase;

    obj.player_1 = createPlayerObject();
    obj.player_1.blockedPenguins = 0;

    obj.player_2 = createPlayerObject();
    obj.player_2.blockedPenguins = 0;

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

        if (getCurrentPlayer(game)->blockedPenguins == game->numberOfPenguins)
        {
            printfMagenta("All of %s's penguins are blocked. He loses his turn", getCurrentPlayer(game)->name);
            game->playerTurn = game->playerTurn ^ 1;
            return;
        }

        printf("\n%s's turn to move. First choose a penguin (x, y): ", getCurrentPlayer(game)->name);
        scanf("%d %d", &penguinX, &penguinY);

        penguinX--;
        penguinY--;

        struct GridPoint *initialPoint = getValidatedChoicePoint(game, penguinX, penguinY);
        if (initialPoint == NULL)
            return;

        initialPoint->selected = true;

        system("cls");
        game->displayUI(game); // we gotta rerender everything so that the
        // chosen point will be marked with a yellow background

        struct GridPoint *destination;

        for (;;)
        {
            printf("\n%s's turn to move. Choose where to go (x, y): ", getCurrentPlayer(game)->name);
            scanf("%d %d", &toX, &toY);
            toX--;
            toY--;

            destination = getValidatedMovePoint(game, initialPoint, toX, toY);
            if (destination != NULL)
                break;

            system("cls");
            game->displayUI(game);
        }

        moveAPenguin(game, initialPoint, destination);

        break;
    }
    }
}

bool isPointInBounds(const struct GameGrid *gameGrid, const int x, const int y)
{
    return x >= 0 && x < gameGrid->rows && y >= 0 && y < gameGrid->cols;
}

struct GridPoint *getValidatedChoicePoint(struct GameSystem *game, const int x, const int y)
{
    if (!isPointInBounds(&game->GameGrid, x, y))
    {
        printfError("The following coordinates exceed the grid size!");
        return NULL;
    }
    struct GridPoint *point = &game->GameGrid.grid[x][y];

    if (point->owner == NULL)
    {
        printfError("This is not a penguin");
        return NULL;
    }

    if (point->owner != getCurrentPlayer(game))
    {
        char t[] = "This penguin belongs to ";
        strcat(t, (game->playerTurn == 0 ? game->player_2.name : game->player_1.name));
        printfError(t);
        return NULL;
    }

    if (point->penguinBlocked)
    {
        printfError("Unable to choose this penguin as it is blocked");
        return NULL;
    }

    return point;
}

struct GridPoint *getValidatedMovePoint(struct GameSystem *game, struct GridPoint *initialPoint, const int x, const int y)
{
    if (!isPointInBounds(&game->GameGrid, x, y))
    {
        printfError("The following coordinates exceed the grid size!");
        return NULL;
    }
    struct GridPoint *point = &game->GameGrid.grid[x][y];

    if (point->owner != NULL)
    {
        printfError("This tile is already occupied by another penguin");
        return NULL;
    }
    if (point->removed)
    {
        printfError("This tile has already been cleared. Try a different one");
        return NULL;
    }
    if (initialPoint->x != point->x && initialPoint->y != point->y)
    {
        printfError("Diagonal moves are not allowed");
        return NULL;
    }

    int moveToCoordinate = (initialPoint->x != point->x ? point->x : point->y);
    int initialCoordinate = (initialPoint->x != point->x ? initialPoint->x : initialPoint->y);

    bool movingInX = initialPoint->x != point->x;

    struct GameGrid *grid = &game->GameGrid;

    if (moveToCoordinate > initialCoordinate)
    {
        for (int i = initialCoordinate + 1; i < moveToCoordinate; i++)
        {
            struct GridPoint *t;
            if (movingInX)
                t = &grid->grid[i][point->y];
            else
                t = &grid->grid[point->x][i];
            if (t->removed)
            {
                printfError("You cannot move across empty tiles and other penguins");
                return NULL;
            }
            if (t->owner != NULL)
            {
                printfError("You cannot move across empty tiles and other penguins");
                return NULL;
            }
        }
    }
    else
    {
        for (int i = initialCoordinate - 1; i > moveToCoordinate; i--)
        {
            struct GridPoint *t;
            if (movingInX)
                t = &grid->grid[i][point->y];
            else
                t = &grid->grid[point->x][i];
            if (t->removed)
            {
                printfError("You cannot move across empty tiles and other penguins");
                return NULL;
            }
            if (t->owner != NULL)
            {
                printfError("You cannot move across empty tiles and other penguins");
                return NULL;
            }
        }
    }

    return point;
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

void moveAPenguin(struct GameSystem *game, struct GridPoint *initialPoint, struct GridPoint *destination)
{
    initialPoint->removed = true;
    initialPoint->owner = NULL;
    strcpy(initialPoint->label, "  ");

    getCurrentPlayer(game)->collectedFishes += destination->numberOfFishes;
    strcpy(destination->label, game->playerTurn == 0 ? "P1" : "P2");
    destination->owner = getCurrentPlayer(game);

    game->playerTurn = game->playerTurn ^ 1;

    checkForBlockedPenguins(&game->GameGrid);
}

bool canMoveToPoint(struct GameGrid *gameGrid, const int x, const int y)
{
    return isPointInBounds(gameGrid, x, y) && gameGrid->grid[x][y].owner == NULL && gameGrid->grid[x][y].removed == false;
}

void checkForBlockedPenguins(struct GameGrid *gameGrid)
{
    for (int x = 0; x < gameGrid->rows; x++)
    {
        for (int y = 0; y < gameGrid->cols; y++)
        {
            if (gameGrid->grid[x][y].owner == NULL)
                continue;

            if (!(canMoveToPoint(gameGrid, x + 1, y) || canMoveToPoint(gameGrid, x - 1, y) ||
                  canMoveToPoint(gameGrid, x, y + 1) ||
                  canMoveToPoint(gameGrid, x, y - 1)))
            {
                gameGrid->grid[x][y].penguinBlocked = true;
                gameGrid->grid[x][y].owner->blockedPenguins++;
            }
        }
    }
}
