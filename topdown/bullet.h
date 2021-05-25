#ifndef BULLET_H
#define BULLET_H
#include "sdlinclude.h"
#include "world.h"
#include <stdbool.h>

#define BULLET_SPEED 15

typedef struct Bullet_type* Bullet;

Bullet createBullet();
void spawnBullet(Bullet a, int xOrigin, int yOrigin, double angle, int damage);
double getBulletDirection(Bullet a);
bool isBulletActive(Bullet bullet);
SDL_Rect* getBulletRect(Bullet bullet);
void freeBullet(Bullet a);
int getBulletX(Bullet a);
int getBulletY(Bullet a);
int getBulletDamage(Bullet b);
void bulletActivate(Bullet b);
void simulateBullets(Bullet aBullets[][MAX_BULLETS]);
bool bulletHit(Bullet b);
int getBulletHitValue(Bullet b);
bool bulletShot(Bullet a);
int getBulletOriginX(Bullet a);
int getBulletOriginY(Bullet a);
bool checkShot(Bullet a);
#endif
