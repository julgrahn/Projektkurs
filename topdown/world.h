#pragma once
#ifndef world_h
#define world_h
#include <stdio.h>

typedef struct CreateWall* Wall;

Wall CreateWall(int w, int h, int wPX, int wPY);

int getTileRows();
int getTileColumns();
int getTileWidth();
int getTileHeight();
int getTileGrid(int x, int y);
int getWallPos(Wall wall);

#endif /* world_h */
