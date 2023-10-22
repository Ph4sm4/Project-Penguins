#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "GameGrid/Grid.h"

const int numberOfPenguins = 2;

void setup(int *, int *);
void welcomeLine();

int main()
{
    srand((unsigned int)time(NULL));

    struct GameGrid grid = createGameGridObject();
    setup(&grid.rows, &grid.cols);
    grid.constructGrid(&grid);
    grid.printGridState(&grid);

    return 0;
}

void setup(int *rows, int *cols)
{
    welcomeLine();
    printf("Enter the number of rows: ");
    scanf("%d", rows);
    printf("Enter the number of columns: ");
    scanf("%d", cols);
}

void welcomeLine()
{
    printf("\n---- PROJECT \"PENGUINS\" ----\n\n");
}
