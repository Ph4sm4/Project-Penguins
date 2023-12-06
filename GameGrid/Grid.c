#include "Grid.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "time.h"
#include "DisplayManager.h"
#include "../Player/Player.h"

// =========================================
// available public functions:

struct GameGrid createGameGridObject();
void constructGrid(struct GameGrid *obj);
void printGridState(const struct GameGrid *obj, const enum GameState phase);
int getGridSize(struct GameGrid *obj);
void checkForBlockedPenguins(struct GameGrid *gameGrid);
bool isPointInBounds(const struct GameGrid *gameGrid, const int x, const int y);

// private functions:

// generates perlin noise values, in-depth explanation in the body of the function
void generatePerlinNoise2D(int nWidth, int nHeight, float *baseSeed, int nOctaves, float *noiseOutput, float bias);

//
bool canMoveToPoint(struct GameGrid *gameGrid, const int x, const int y);

// =========================================

struct GameGrid createGameGridObject()
{
    struct GameGrid obj;
    obj.constructGrid = &constructGrid;
    obj.printGridState = &printGridState;
    obj.getGridSize = &getGridSize;
    obj.checkForBlockedPenguins = &checkForBlockedPenguins;
    obj.isPointInBounds = &isPointInBounds;

    return obj;
}

int getGridSize(struct GameGrid *obj)
{
    return obj->rows * obj->cols;
}

void constructGrid(struct GameGrid *obj)
{
    const int rows = obj->rows;
    const int cols = obj->cols;

    // allocate memory for array of pointers (to another arrays)
    obj->grid = (struct GridPoint **)malloc(rows * sizeof(struct GridPoint *));
    // initialize memory for arrays of pointers (to GridPoint objects)
    for (int i = 0; i < rows; i++)
    {
        obj->grid[i] = (struct GridPoint *)malloc(cols * sizeof(struct GridPoint));
    }

    const int gridSize = obj->rows * obj->cols;
    const int perlin2DArrayW = 256; // a 2d array 256x256, smaller arrays dont really make any sense
    const int perlin2DArraySize = perlin2DArrayW * perlin2DArrayW;

    // allocating memory for arrays needed to generate the noise
    obj->noiseSeed2D = (float *)malloc(perlin2DArraySize * sizeof(float));
    obj->perlinNoise2D = (float *)malloc(perlin2DArraySize * sizeof(float));

    // populating noiseSeed with random numbers, range 0 to 1
    for (int i = 0; i < perlin2DArraySize; i++)
    {
        obj->noiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
    }

    // the number of iterations of the algorithm that we want to apply, maximum is log2(base seed array width)
    const int maxOctave = ceil(log2(perlin2DArrayW));

    // printf("Choose how often should higher numbers of fish spawn (1-10). Higher number will decrease the occurance of valuable fields: ");
    // float tempBias;
    // scanf("%f", &tempBias);
    const float scalingBias = 0.20f;
    generatePerlinNoise2D(perlin2DArrayW, perlin2DArrayW, obj->noiseSeed2D, maxOctave, obj->perlinNoise2D, scalingBias);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            /* TO BE DISCUSSED WHY THIS WON'T WORK AS I WOULD EXPECT IT TO
            // int sectionWidth = perlin2DArrayW / rows;
            // int sectionHeight = perlin2DArrayW / cols; // both are gonna get floored so no overflow possible

            // int noiseFishSum = 0;
            // split the perlin noise array which is likely much larger than our grid into smaller sections
            // and calculate the average noise value from each section -> this way we are essentially gonna scale down the perlinNoise array;
            // we couldnt have made the perlinNoise array same size as the game grid as generation would not be sufficient enough
            // i.e. (not enough octaves would have been performed)
            // int loopCount = 0;
            // for (int k = i * sectionWidth; k < i * sectionWidth + sectionWidth; k++)
            // {
            //     for (int l = j * sectionHeight; l < j * sectionHeight + sectionHeight; l++)
            //     {
            //         noiseFishSum += (int)(obj->perlinNoise2D[l * sectionWidth + k] * 10.0f);
            //         loopCount++;
            //     }
            // }
            */

            int noiseFishNumber = (int)(obj->perlinNoise2D[j * rows + i] * 10.0f);

            struct GridPoint *p = &obj->grid[i][j];

            (*p).x = i;
            (*p).y = j;
            (*p).label = (char *)malloc(strlen("##") + sizeof(char)); // + sizeof(char) for the null terminator
            (*p).owner = NULL;
            (*p).removed = false;
            (*p).selected = false;
            (*p).penguinBlocked = false;

            if (noiseFishNumber < 4)
            {
                strcpy((*p).label, "##");
                (*p).numberOfFishes = 0;
            }
            else if (noiseFishNumber >= 4 && noiseFishNumber < 6)
            {
                strcpy((*p).label, "#1");
                (*p).numberOfFishes = 1;
            }
            else if (noiseFishNumber >= 6 && noiseFishNumber < 8)
            {
                strcpy((*p).label, "#2");
                (*p).numberOfFishes = 2;
            }
            else if (noiseFishNumber >= 8 && noiseFishNumber <= 10)
            {
                strcpy((*p).label, "#3");
                (*p).numberOfFishes = 3;
            }
        }
    }
}

void printGridState(const struct GameGrid *obj, const enum GameState phase)
{
    // print 8 spaces before column numbers
    printf("\n\n\n");
    printf("%*sY%*s", 3, "", 3, "");

    int numSpaces;
    // print column indexes
    for (int i = 0; i < obj->cols; i++)
    {
        numSpaces = 4 - (int)log10(i + 1);
        printf("%d%*s", i + 1, numSpaces, "");
    }
    printf("\n%*s", 5, "");
    // print column separators
    for (int i = 0; i < obj->cols * 5; i++)
    {
        printf("-");
    }
    printf("\nX\n");

    for (int i = 0; i < obj->rows; i++)
    {
        numSpaces = 4 - (int)log10(i + 1); // this expression gets the number of number of digits in a character (for 1234 we get length of 4)

        printf("%d%*s| ", i + 1, numSpaces, "");

        for (int j = 0; j < obj->cols; j++)
        {
            numSpaces = 3;
            char *label = obj->grid[i][j].label;
            int fishNum = obj->grid[i][j].numberOfFishes;
            if (phase == (enum GameState)PlacingPhase)
            {
                if (!strcmp(label, "P1"))
                {
                    printfOrange("%s%*s", label, numSpaces, "");
                }
                else if (!strcmp(label, "P2"))
                {
                    printfGreen("%s%*s", label, numSpaces, "");
                }
                else if (fishNum == 1)
                {
                    printfYellow("%s%*s", label, numSpaces, "");
                }
                else
                {
                    printfGray("%s%*s", label, numSpaces, "");
                }
            }
            else
            {
                if (obj->grid[i][j].removed && !obj->grid[i][j].owner)
                {
                    printf("%s%*s", label, numSpaces, "");
                }
                else if (obj->grid[i][j].penguinBlocked)
                {
                    printfGray("%s%*s", label, numSpaces, "");
                }
                else if ((!strcmp(label, "P1") || !strcmp(label, "P2")) && obj->grid[i][j].selected)
                {
                    printfOrangeNBackgroundYellow("%s", label);
                    printf("%*s", numSpaces, "");
                }
                else if (!strcmp(label, "P1"))
                {
                    printfOrange("%s%*s", label, numSpaces, "");
                }
                else if (!strcmp(label, "P2"))
                {
                    printfGreen("%s%*s", label, numSpaces, "");
                }
                else
                {
                    printfCyan("%s%*s", label, numSpaces, "");
                }
            }
        }
        printf("\n");
    }
}

void generatePerlinNoise2D(int nWidth, int nHeight, float *baseSeed, int nOctaves, float *noiseOutput, float bias)
{

    /*
     * baseSeed - base seed array of random values between 0 and 1
     * nOctaves - the number of iterations of the algorithm that we want to apply, maximum is log2(base seed array size)
     * noiseOutput - a pointer to the output array
     */
    for (int x = 0; x < nWidth; x++)
    {
        for (int y = 0; y < nHeight; y++)
        {
            float noiseValue = 0.f;

            float scalingFactorAcc = 0.f;
            float scalingFactor = 1.0f;

            for (int j = 0; j < nOctaves; j++)
            {
                // shifting bits to the right -> dividing by half/two each octave
                int pitch = nWidth >> j;
                /*
                 * the pitch is basically responsible for deciding how often do we want to choose a point, that is gonna enable us to calculate the value,
                 * this way we can get more and more satisfing results
                 */

                int pointX1 = (x / pitch) * pitch; // the first point is always going to be <= i
                int pointY1 = (y / pitch) * pitch;

                int pointX2 = (pointX1 + pitch) % nWidth;
                int pointY2 = (pointY1 + pitch) % nWidth; // modulo - because we want to loop around if we overflow the array

                /*
                 * it is obligatory to choose 2 points, as in that one iteration we are calculating values for one single point, or element in the array,
                 * as we know - we are "drawing" an invisible straight line between each value, this is why we can use linear interpolation between the
                 * values from the seed array to get the proper noise value of that one particular cell
                 *
                 * as the octaves will grow, the lines will become shorter and shorter, to the point in which there is going to be a line between each calculated value
                 * from the seed array
                 */

                float HowFarIntoLineX = (float)(x - pointX1) / (float)pitch;
                float HowFarIntoLineY = (float)(y - pointY1) / (float)pitch;
                /*
                 * we need to know, how far are we into the invisible line between our current point (which we are calculating)
                 * and the point that we have information about (the point that was chosen basing on the pitch, so how often
                 * do we pick a point, and the information is simply its value)
                 *
                 * (float)(i - firstPoint) this will give us the value between 0 and the pitch
                 *
                 * for example, if current point's index is 87, and the octave is 3, then for our case (nCount/size of the array = 256)
                 * the pitch is gonna be 256 / 2 / 2 which is 64, so upper expression will give us
                 * (87 - 64 = 23) / 64 -> 0.359375
                 * and that is how far are we into the "line" so using linear interpolation, we can now calculate the proper value of that point
                 */

                float InterpolatedX = (1.0f - HowFarIntoLineX) * baseSeed[pointY1 * nWidth + pointX1] + HowFarIntoLineX * baseSeed[pointY1 * nWidth + pointX2];
                float InterpolatedY = (1.0f - HowFarIntoLineX) * baseSeed[pointY2 * nWidth + pointX1] + HowFarIntoLineX * baseSeed[pointY2 * nWidth + pointX2];

                // now we can accumulate calculated values
                noiseValue += (HowFarIntoLineY * (InterpolatedY - InterpolatedX) + InterpolatedX) * scalingFactor;

                scalingFactorAcc += scalingFactor;
                scalingFactor /= bias;
            }

            noiseOutput[y * nWidth + x] = noiseValue / scalingFactorAcc;
            /*
             * by dividing by the scalingFactorAcc we are making sure that the value range of our points is gonna
             * be from 0 to 1
             */
        }
    }
}

void checkForBlockedPenguins(struct GameGrid *gameGrid)
{
    for (int x = 0; x < gameGrid->rows; x++)
    {
        for (int y = 0; y < gameGrid->cols; y++)
        {
            if (gameGrid->grid[x][y].owner == NULL)
                continue;

            if (!gameGrid->grid[x][y].penguinBlocked && !(canMoveToPoint(gameGrid, x + 1, y) || canMoveToPoint(gameGrid, x - 1, y) ||
                                                          canMoveToPoint(gameGrid, x, y + 1) ||
                                                          canMoveToPoint(gameGrid, x, y - 1)))
            {
                gameGrid->grid[x][y].penguinBlocked = true;
                gameGrid->grid[x][y].owner->blockedPenguins++;
            }
        }
    }
}

bool canMoveToPoint(struct GameGrid *gameGrid, const int x, const int y)
{
    return isPointInBounds(gameGrid, x, y) && gameGrid->grid[x][y].owner == NULL && gameGrid->grid[x][y].removed == false;
}

bool isPointInBounds(const struct GameGrid *gameGrid, const int x, const int y)
{
    return x >= 0 && x < gameGrid->rows && y >= 0 && y < gameGrid->cols;
}