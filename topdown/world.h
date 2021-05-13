#pragma once
#ifndef world_h
#define world_h
#include <stdio.h>
#include <stdbool.h>

int getTileRows();
int getTileColumns();
int getTileWidth();
int getTileHeight();
int getTileGrid(int x, int y);
int getWallCollisionPlayer(int x, int y);
bool getWallCollisionBullet(int x, int y, int h, int w);
void countWallHits(int i, int j);
int getTileGridHits(int x, int y);

#endif /* world_h */