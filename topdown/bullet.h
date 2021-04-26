#ifndef BULLET_H
#define BULLET_H
#include "sdlinclude.h"
#include <stdbool.h>

typedef struct Bullet_type* Bullet;

Bullet createBullet();
void spawnBullet(Bullet bullet, int x, int y, int direction, int owner);
bool isBulletActive(Bullet bullet);
void moveBullet(Bullet bullet);
SDL_Rect* getBulletRect(Bullet bullet);
void freeBullet(Bullet a);
int getBulletOwner(Bullet b);
#endif
