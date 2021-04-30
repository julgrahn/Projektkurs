<<<<<<< Updated upstream
#include "world.h"
=======
﻿#include "world.h"
#include "player.h"

>>>>>>> Stashed changes

enum material { bricks = 120, wall = 186 };

int tileGrid[22][22] = {
    {wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
<<<<<<< Updated upstream
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
=======
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,wall,wall,wall,wall,wall,wall,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
>>>>>>> Stashed changes
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,wall,wall,wall,wall,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
};

int tileRows = 22;
int tileColumns = 22;

int tileWidth = 32;
int tileHeight = 32;

int getTileRows() {
    return tileRows;
}
int getTileColumns() {
    return tileColumns;
}
int getTileWidth() {
    return tileWidth;
}
int getTileHeight() {
    return tileHeight;
}
int getTileGrid(int x, int y) {
    return tileGrid[x][y];
}
<<<<<<< Updated upstream
int getWall(int x, int y, int h, int w){
=======
int getWallCollision(int x, int y) {
    int wallCordStartX;
    int wallCordEndX;
    int wallCordStartY;
    int wallCordEndY;
>>>>>>> Stashed changes
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            if (tileGrid[i][j] != bricks)
            {
<<<<<<< Updated upstream
=======
                wallCordStartX = j * 32;
                wallCordEndX = j * 32 + 32;
                wallCordStartY = i * 32;
                wallCordEndY = i * 32 + 32;

                if (x + 64 >= wallCordStartX + 20 && x <= wallCordEndX && y + 64 >= wallCordStartY + 15 && y <= wallCordEndY - 15) // addition och sub i IF sats baseras p� spelarrektangel och kan beh�va anpassas. 
                {
                    // if (x < wallCordStartX)
                    // {
                    //     printf("%d\n", wallCordStartX-x); // 43-44
                    //     return 1;
                    // }
                    // if (x + 1 >= wallCordEndX)
                    // {
                    //     printf("%d\n",x-wallCordEndX); 
                    //     return 1;
                    // }
                    // if (y < wallCordStartY)
                    // {
                    //     printf("%d\n",wallCordStartY-y); // 48-49
                    //     return 2;
                    // }
                    // if (y + 16 > wallCordEndY)
                    // {
                    //     printf("%d\n",y-wallCordEndY);
                    //     return 2;
                    // }
                    return 1;
>>>>>>> Stashed changes

                printf("kolumn: %d, rad: %d\n", j*32, i*32);
                printf("kolumn: %d, rad: %d\n", j*32+32, i*32+32);
                
            }
        }
    }
    return 0;
}