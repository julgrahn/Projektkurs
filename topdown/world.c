#include "world.h"
#include "player.h"


enum material {bricks = 120, wall = 186};

int tileGrid[22][22] = {
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
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
int getWallCollision(int x, int y){
    int wallCordStartX;
    int wallCordEndX;
    int wallCordStartY;
    int wallCordEndY;
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            if (tileGrid[i][j] != bricks)
            {
                wallCordStartX = j * 32;
                wallCordEndX = j*32 + 32; 
                wallCordStartY = i * 32;
                wallCordEndY = i*32 + 32;

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