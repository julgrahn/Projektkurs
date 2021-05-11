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
	int hit;
	int shotTimer;
	bool shot;
	int xOrigin, yOrigin;
};

PUBLIC Bullet createBullet()
{
	Bullet b = malloc(sizeof(struct Bullet_type));
	b->active = false;
	b->dimensions.w = 10;
	b->dimensions.h = 10;
	b->speed = BULLET_SPEED;
	b->damage = BULLET_DAMAGE;
	b->hit = 0;
	b->shotTimer = 0;
	b->shot = false;
	return b;
}

PUBLIC void spawnBullet2(Bullet a, int xOrigin, int yOrigin, double angle)
{
	a->active = 1;
	a->xPos = a->xOrigin = xOrigin;
	a->yPos = a->yOrigin = yOrigin;
	a->xSpeed = a->speed * cos(angle);
	a->ySpeed = a->speed * sin(angle);
	a->hit = 0;
	a->shotTimer = 5;
	a->shot = true;
}

PUBLIC void spawnBullet3(Bullet a, int xOrigin, int yOrigin, double angle, int damage)
{
	a->active = 1;
	a->xPos = a->xOrigin = xOrigin ;
	a->yPos = a->yOrigin = yOrigin ;
	a->xSpeed = a->speed * cos(angle);
	a->ySpeed = a->speed * sin(angle);
	a->hit = 0;
	a->shotTimer = 5;
	a->shot = true;
	a->damage = damage;
}

PUBLIC void spawnBullet(Bullet a, int xOrigin, int yOrigin, int xTarget, int yTarget, int owner, int damage)
{
	a->active = 1;
	a->xPos = a->xOrigin = xOrigin ;
	a->yPos = a->yOrigin = yOrigin ;
	a->direction = atan2(yTarget - (a->yPos + (a->dimensions.h / 2)), xTarget - (a->xPos + (a->dimensions.w / 2)));
	a->xSpeed = a->speed * cos(a->direction);
	a->ySpeed = a->speed * sin(a->direction);
	a->owner = owner;
	a->hit = 0;
	a->shotTimer = 5;
	a->shot = true;
	a->damage = damage;
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
	a->hit = 3;
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

PUBLIC bool bulletHit(Bullet b)
{
	if(b->hit)
	{
		b->hit--;
		return true;
	}
	else return false;
}

PUBLIC bool bulletShot(Bullet a)
{
	if(a->shotTimer)
	{
		a->shotTimer--;
		return true;
	}
	else return false;
}

PUBLIC int getBulletOriginX(Bullet a)
{
	return a->xOrigin;
}

PUBLIC int getBulletOriginY(Bullet a)
{
	return a->yOrigin;
}

PUBLIC bool checkShot(Bullet a)
{
	if(a->shot)
	{
		a->shot = false;
		return true;
	}
	else return false;
}