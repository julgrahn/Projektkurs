#include "world.h"
#include "player.h"

#define PUBLIC
#define PRIVATE static

PRIVATE void wallColMultiAngleCompensation(double *xPos, double *yPos, int xWall, int yWall, int minDistance);
PRIVATE void wallColSingleAngleCompensation(double *pos, int wallStart, int wallEnd, int minDistance);

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

PUBLIC void wallPlayerCollisionHandling(double *posX, double *posY, int r)
{
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

                // X-AXIS
                if(*posX+r > wallCordStartX && *posX-r < wallCordEndX && *posY > wallCordStartY && *posY < wallCordEndY)
                    wallColSingleAngleCompensation(posX, wallCordStartX, wallCordEndX, r);
                
                // Y-AXIS
                else if(*posY+r > wallCordStartY && *posY-r < wallCordEndY && *posX > wallCordStartX && *posX < wallCordEndX)
                    wallColSingleAngleCompensation(posY, wallCordStartY, wallCordEndY, r);
                
                // TOP LEFT
                else if(*posX < wallCordStartX && *posY < wallCordStartY)
                    wallColMultiAngleCompensation(posX, posY, wallCordStartX, wallCordStartY, r);
                
                // TOP RIGHT
                else if(*posX > wallCordEndX && *posY < wallCordStartY)
                    wallColMultiAngleCompensation(posX, posY, wallCordEndX, wallCordStartY, r);
            
                // BOTTOM LEFT
                else if(*posX < wallCordStartX && *posY > wallCordEndY)
                    wallColMultiAngleCompensation(posX, posY, wallCordStartX, wallCordEndY, r);

                // BOTTOM RIGHT
                else if(*posX > wallCordEndX && *posY > wallCordEndY)
                    wallColMultiAngleCompensation(posX, posY, wallCordEndX, wallCordEndY, r);
            }
        }
    }
}

PRIVATE void wallColSingleAngleCompensation(double *pos, int wallStart, int wallEnd, int minDistance)
{
    double delta;
    delta = fabs(*pos - wallStart);
    if(delta < fabs(*pos - wallEnd))
        *pos = wallStart - minDistance;
    else *pos = wallEnd + minDistance;
}

PRIVATE void wallColMultiAngleCompensation(double *xPos, double *yPos, int xWall, int yWall, int minDistance)
{
    double xDelta, yDelta;
    double distance = sqrt((round(*xPos) - xWall)*(round(*xPos) - xWall) + (round(*yPos) - yWall)*(round(*yPos) - yWall));
    if(distance < minDistance)
    {
        xDelta = round(*xPos)-xWall;
        yDelta = round(*yPos)-yWall;
        *xPos += (minDistance-distance)*xDelta/distance;
        *yPos += (minDistance-distance)*yDelta/distance;
    }
}

PUBLIC bool getWallCollisionBullet(int x, int y, int h, int w)
{
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