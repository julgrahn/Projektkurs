#include <stdlib.h>
#include "bullet.h"
#include <stdbool.h>
#include <math.h>
#define PUBLIC
#define PRIVATE static
#define BULLET_CENTER_OFFSET_X 4
#define BULLET_CENTER_OFFSET_Y 4

PRIVATE void moveBullet(Bullet bullet);
PRIVATE void bulletWallHit(Bullet a);

struct Bullet_type {
	SDL_Rect dimensions;
	bool active;
	int speed;
	double xPos;
	double yPos;
	double xSpeed;
	double ySpeed;
	double direction;
	int damage;
	int hit; // Used to calculate wall explosion after hit 
	int shotTimer; // Used to calculate fire rate
	bool shot;
	int xOrigin, yOrigin;
	bool hitWall;
};

PUBLIC Bullet createBullet()
{
	Bullet b = malloc(sizeof(struct Bullet_type));
	b->active = false;
	b->dimensions.w = BULLET_CENTER_OFFSET_X;
	b->dimensions.h = BULLET_CENTER_OFFSET_Y;
	b->speed = BULLET_SPEED;
	b->damage = 0;
	b->hit = 0;
	b->shotTimer = 0;
	b->shot = false;
	b->hitWall = false;
	return b;
}
PUBLIC void spawnBullet(Bullet a, int xOrigin, int yOrigin, double angle, int damage)
{
	a->active = 1;
	a->xPos = a->xOrigin = xOrigin;
	a->yPos = a->yOrigin = yOrigin;
	a->direction = angle;
	a->xSpeed = a->speed * cos(angle);
	a->ySpeed = a->speed * sin(angle);
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
		bullet->dimensions.x = round(bullet->xPos)-BULLET_CENTER_OFFSET_X/2;
		bullet->dimensions.y = round(bullet->yPos)-BULLET_CENTER_OFFSET_Y/2;

		// Remove bullet if it hits a wall
		if (getWallCollisionBullet(bullet->dimensions.x, bullet->dimensions.y, bullet->dimensions.h, bullet->dimensions.w))
		{	
			freeBullet(bullet);
			bulletWallHit(bullet);
		}
		
		// Remove bullet if it hits the edge of the screen
		if (bullet->dimensions.x < 0 || bullet->dimensions.x > WINDOWWIDTH || bullet->dimensions.y < 0 || bullet->dimensions.y > WINDOWHEIGHT)
			bullet->active = false;
	}
}

PUBLIC bool isWallhit(Bullet a)
{
	return a->hitWall;
}

PRIVATE void bulletWallHit(Bullet a)
{
	a->hitWall = true;
}

PUBLIC SDL_Rect* getBulletRect(Bullet bullet)
{
	return &bullet->dimensions;
}

PUBLIC void freeBullet(Bullet a)
{
	a->active = false;
	a->hit = 15;
}

PUBLIC double getBulletDirection(Bullet a)
{
	return a->direction;
}

PUBLIC int getBulletX(Bullet a)
{
	return round(a->xPos); //a->dimensions.x;
}

PUBLIC int getBulletY(Bullet a)
{
	return round(a->yPos); //a->dimensions.y;
}

PUBLIC int getBulletDamage(Bullet b)
{
	return b->damage;
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

PUBLIC bool bulletHit(Bullet b)
{
	if(b->hit)
	{
		b->hit--;
		return true;
	}
	else
	{
		b->hitWall = false;
		return false;
	}
}

PUBLIC int getBulletHitValue(Bullet b)
{
	return b->hit;
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