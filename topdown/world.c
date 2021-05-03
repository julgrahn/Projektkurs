#include "world.h"

enum material {bricks = 120, grass = 35, wall = 85};

#define PUBLIC

int tileGrid[22][22] = {
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,bricks,bricks,bricks,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,wall,wall,wall,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
    {bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks,bricks},
};

int tileRows = 22;
int tileColumns = 22;

int tileWidth = 32;
int tileHeight = 32;

struct CreateWall
{
    int wallWidth;
    int wallHeight;
    int wallPosX;
    int wallPosY;

};

PUBLIC Wall CreateWall(int w, int h, int wPX, int wPY)
{
    Wall a = malloc(sizeof(struct CreateWall));

    a->wallHeight = 32;
    a->wallWidth = 32;
    a->wallPosX = wPX, a->wallPosY = wPY;
    return a;
}

int getWallPos(Wall wall)
{
    for (int i = 0; i < tileRows; i++)
    {
        if (tileGrid[i] == "wall")
        {
            printf("wallposX: %d", wall->wallPosX);
            printf("wallposY: %d", wall->wallPosY);
        }
       
    }
}

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



/*
int getWall(int x, int y){
    for (int i = 0; i < tileRows; i++)
    {
        for (int j = 0; j < tileColumns; j++)
        {
            if (tileGrid[i][j] != bricks)
            {
                printf("kolumn: %d, rad: %d\n", j * 32, i * 32);
                printf("kolumn: %d, rad: %d\n", (j - 1) * 32, (i - 1) * 32);
            }
        }
    }

    return 0;
}*/