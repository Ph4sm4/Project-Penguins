#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "Windows.h"
#include "GameSystem/GameSystem.h"
#include "Enums/GameState.h"

int main()
{
    srand((unsigned int)time(NULL));

    struct GameSystem Game = createGameSystemObject();

    Game.setup(&Game);

    for (;;)
    {
        Game.displayUI(&Game);
        Game.askForAction(&Game);

        system("cls");
    }

    return 0;
}
