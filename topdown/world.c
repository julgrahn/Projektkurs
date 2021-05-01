#include "world.h"
#include "player.h"
#include <stdbool.h>


enum material {bricks = 120, wall = 186, truck = 102, wall2 = 246, wall3 = 216};

int tileGrid[22][22] = {
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,wall,wall,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,wall,wall,wall,wall,wall,bricks,bricks,wall,wall,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,wall,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks},
    {bricks,bricks,wall,wall,wall,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,wall,wall,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,wall,bricks,bricks,bricks,wall,bricks,bricks,wall,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,wall,wall,wall,wall,wall,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
};

int tileGridHits[22][22] = { 0 };

int wallCordStartX;
int wallCordEndX;
int wallCordStartY;
int wallCordEndY;

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
int getWallCollisionPlayer(int x, int y){
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            if (tileGrid[i][j] != bricks)
            {
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
                }
            }
        }
    }
    return 0;
}
bool getWallCollisionBullet(int x, int y, int h, int w){
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            if (tileGrid[i][j] != bricks)
            {
                wallCordStartX = j * 32;
                wallCordEndX = j * 32 + 32; 
                wallCordStartY = i * 32;
                wallCordEndY = i * 32 + 32;

                if (x + w >= wallCordStartX && x <= wallCordEndX && y + h >= wallCordStartY && y <= wallCordEndY)
                {
                    countWallHits(i, j);
                    return true;
                }
            }
        }
    }
    return false;
}

void countWallHits(int i, int j)
{
    tileGridHits[i][j]++;
    if (tileGridHits[i][j] == 3)
    {
        tileGrid[i][j] = wall3;
    }
    else if (tileGridHits[i][j] == 6)
    {
        tileGrid[i][j] = bricks;
    }
    return;
}