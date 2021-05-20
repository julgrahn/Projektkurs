#include "world.h"
#define PUBLIC



enum material { sp = 121, br = 120, wa = 186, wall2 = 246, wall3 = 216 };
// enum material { br = 222, wall = 186, truck = 102, wall2 = 246, wall3 = 216 };

int tileGrid[24][36] = {
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,sp,br,br,sp,br,br,br,br,wa,wa,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,sp,br,br,sp,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,sp,sp,sp,sp,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br},
    {br,br,sp,sp,sp,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,sp,sp,sp,br,br},
    {br,br,br,br,sp,br,br,br,wa,wa,wa,wa,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,wa,br,br,br,br,sp,br,br,br,br},
    {br,br,br,br,sp,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,sp,br,br,br,br},
    {br,br,sp,sp,sp,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,sp,sp,sp,br,br},
    {br,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,wa,wa,wa,wa,br,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,wa,wa,wa,wa,wa,wa,wa,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br},
    {wa,wa,wa,wa,wa,wa,br,br,br,br,br,br,wa,wa,wa,wa,wa,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,wa,wa,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,sp,sp,sp,sp,br,br,br,br,br,wa,br,br,br,br,br,br,br,br,br,sp,sp,sp,sp,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,sp,br,br,sp,br,br,br,br,br,wa,wa,wa,wa,br,br,br,br,br,br,sp,br,br,sp,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,sp,br,br,sp,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,sp,br,br,sp,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br},
    {br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br,br},
};

int tileGridHits[24][36] = { 0 };

int tilegridReset[24][36];

int wallCordStartX;
int wallCordEndX;
int wallCordStartY;
int wallCordEndY;

int tileRows = 24;
int tileColumns = 36;

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
int getTileGridHits(int x, int y) {
    return tileGridHits[x][y];
}
PUBLIC int getWallCollisionPlayer(int x, int y) {
    int wallCordStartX;
    int wallCordEndX;
    int wallCordStartY;
    int wallCordEndY;
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            if (tileGrid[i][j] != br)
            {
                wallCordStartX = j * 32;
                wallCordEndX = j * 32 + 32;
                wallCordStartY = i * 32;
                wallCordEndY = i * 32 + 32;

                if (x + 64 >= wallCordStartX + 20 && x <= wallCordEndX && y + 64 >= wallCordStartY + 15 && y <= wallCordEndY - 15) // addition och sub med konstanterna i IF sats baseras p� spelarrektangel och kan beh�va anpassas. 
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}
PUBLIC bool getWallCollisionBullet(int x, int y, int h, int w) {
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            if (tileGrid[i][j] != br)
            {
                wallCordStartX = j * 32;
                wallCordEndX = j * 32 + 32;
                wallCordStartY = i * 32;
                wallCordEndY = i * 32 + 32;

                if (x + w >= wallCordStartX && x <= wallCordEndX && y + h >= wallCordStartY && y <= wallCordEndY)
                {
                    if(tileGrid[i][j] == wa || tileGrid[i][j] == wall2 || tileGrid[i][j] == wall3)
                    {
                        countWallHits(i, j);  // Ta bort kommentaren om du vill förstöra väggar
                    }
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
    if (tileGridHits[i][j] == 70)
    {
        tileGrid[i][j] = wall2;
    }
    else if (tileGridHits[i][j] == 140)
    {
        tileGrid[i][j] = wall3;
    }
    else if (tileGridHits[i][j] == 200)
    {
      tileGrid[i][j] = br;
    }
    return;
}

// skapar kopia av orginalkarta
void initTileGridReset()
{
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            tilegridReset[i][j] = tileGrid[i][j];
        }
    }
}

void resetTileGridMap()
{
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            tileGrid[i][j] = tilegridReset[i][j];
        }
    }
}