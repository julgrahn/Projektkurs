#include <stdlib.h>
#include "sdlinclude.h"
#include "bullet.h"
#include "world.h"
#include <stdbool.h>
#include <math.h>
#define PUBLIC
#define PRIVATE static

PRIVATE void moveBullet(Bullet bullet);

struct Bullet_type {
	SDL_Rect dimensions;
	bool active;
	int speed;
	double xPos;
	double yPos;
	double xSpeed;
	double ySpeed;
	int owner;
	double direction;
	int damage;
};

PUBLIC Bullet createBullet()
{
	Bullet b = malloc(sizeof(struct Bullet_type));
	b->active = false;
	b->dimensions.w = 4;
	b->dimensions.h = 4;
	b->speed = BULLET_SPEED;
	b->damage = BULLET_DAMAGE;
	return b;
}

PUBLIC void spawnBullet(Bullet a, int xOrigin, int yOrigin, int xTarget, int yTarget, int owner)
{
	a->active = 1;
	a->xPos = xOrigin + 20;
	a->yPos = yOrigin + 32;
	a->direction = atan2(yTarget - (a->yPos + (a->dimensions.h / 2)), xTarget - (a->xPos + (a->dimensions.w / 2)));
	a->xSpeed = a->speed * cos(a->direction);
	a->ySpeed = a->speed * sin(a->direction);
	a->owner = owner;
}

PUBLIC bool isBulletActive(Bullet bullet)
{
	return bullet->active;
}

PRIVATE void moveBullet(Bullet bullet)
{
	if(isBulletActive(bullet))
	{
		bullet->xPos += bullet->xSpeed;
		bullet->yPos += bullet->ySpeed;

		if (getWallCollisionBullet(bullet->xPos, bullet->yPos, bullet->dimensions.h, bullet->dimensions.w))
		{
			freeBullet(bullet);
		}

		bullet->dimensions.x = round(bullet->xPos);
		bullet->dimensions.y = round(bullet->yPos);

		if (bullet->dimensions.x < 0 || bullet->dimensions.x > WINDOWWIDTH || bullet->dimensions.y < 0 || bullet->dimensions.y > WINDOWHEIGHT)
			bullet->active = false;
	}
}

PUBLIC SDL_Rect* getBulletRect(Bullet bullet)
{
	return &bullet->dimensions;
}

PUBLIC void freeBullet(Bullet a)
{
	a->active = false;
}

PUBLIC double getBulletDirection(Bullet a)
{
	return a->direction;
}

int getBulletOwner(Bullet b)
{
	return b->owner;
}

PUBLIC int getBulletX(Bullet a)
{
	return a->xPos;
}

PUBLIC int getBulletY(Bullet a)
{
	return a->yPos;
}
PUBLIC int getBulletDamage(Bullet b)
{
	return b->damage;
}

PUBLIC void setBulletXY(Bullet b, int x, int y)
{
	b->xPos = b->dimensions.x = x, b->yPos = b->dimensions.y = y; 
}

PUBLIC void bulletActivate(Bullet b)
{
	b->active = true;
}

PUBLIC void simulateBullets(Bullet aBullets[][MAX_BULLETS])
{
	for (int i = 0; i < MAX_PLAYERS; i++)
		for(int j = 0; j < MAX_BULLETS; j++)
			moveBullet(aBullets[i][j]);
}

PUBLIC void setBulletSpeed(Bullet b, double x, double y)
{
	b->xSpeed = x, b->ySpeed = y;
}