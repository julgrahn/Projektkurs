// #pragma once
#ifndef world_h
#define world_h
#include <stdio.h>
#include <stdbool.h>

#define TILE_ROWS 22
#define TILE_COLUMNS 22
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

int getTileRows();
int getTileColumns();
int getTileWidth();
int getTileHeight();
int getTileGrid(int x, int y);
void wallPlayerCollisionHandling(double *posX, double *posY, int r);
bool getWallCollisionBullet(int x, int y, int h, int w);
void countWallHits(int i, int j);
int getTileGridHits(int x, int y);

bool getWallCollisionBullet(int x, int y, int h, int w);
void initTileGridReset();
void resetTileGridMap();

#endif /* world_h */