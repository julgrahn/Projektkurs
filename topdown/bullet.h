#ifndef BULLET_H
#define BULLET_H
#include "sdlinclude.h"
#include <stdbool.h>

typedef struct Bullet_type* Bullet;

Bullet createBullet();
SDL_Rect* getBulletRect(Bullet bullet);
// void spawnBullet(Bullet bullet, int x, int y, double direction);
void spawnBullet(Bullet a, int xOrigin, int yOrigin, int xTarget, int Ytarget, int owner);
double getBulletDirection(Bullet a);
bool isBulletActive(Bullet bullet);
void moveBullet(Bullet bullet);
void freeBullet(Bullet a);
int getBulletOwner(Bullet b);
int getBulletX(Bullet a);
int getBulletY(Bullet a);

int getBulletDamage(Bullet b);
#endif
