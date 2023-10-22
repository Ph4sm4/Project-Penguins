#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "GameSystem/GameSystem.h"

int main()
{
    srand((unsigned int)time(NULL));

    struct GameSystem Game = createGameSystemObject();

    Game.setup(&Game);

    Game.displayUI(&Game);

    return 0;
}
