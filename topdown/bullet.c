#include <stdlib.h>
#include "sdlinclude.h"
#include "bullet.h"
#include <stdbool.h>
#include <math.h>

#define BULLET_SPEED 10
#define PUBLIC

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
};

PUBLIC Bullet createBullet()
{
    Bullet b = malloc(sizeof(struct Bullet_type));
	b->active = false;
	b->dimensions.w = 4;
	b->dimensions.h = 4;
	b->speed = BULLET_SPEED;
  
    return b;
}

PUBLIC void spawnBullet(Bullet a, int xOrigin, int yOrigin, SDL_Point target)
{
	a->active = 1;
	a->xPos = xOrigin + 20;
	a->yPos = yOrigin + 32;
	a->direction = atan2(target.y - (a->yPos+(a->dimensions.h/2)), target.x - (a->xPos+(a->dimensions.w/2)));
	a->xSpeed = a->speed * cos(a->direction);
	a->ySpeed = a->speed * sin(a->direction);
}

// PUBLIC void spawnBullet(Bullet bullet, int x, int y, double direction)
// {
// 	bullet->active = 1;
// 	bullet->xPos = x + 20;
// 	bullet->yPos = y + 32;	
// 	bullet->direction = direction * M_PI / 180;

// 	//printf("xSpeed: %d\n", bullet->xSpeed);
// 	//printf("ySpeed: %d\n", bullet->ySpeed);

// 	bullet->xSpeed = bullet->speed * cos(bullet->direction);
// 	bullet->ySpeed = bullet->speed * sin(bullet->direction);

// }

PUBLIC bool isBulletActive(Bullet bullet)
{
	return bullet->active;
}

PUBLIC void moveBullet(Bullet bullet)
{
	

	bullet->xPos += bullet->xSpeed;
	bullet->yPos += bullet->ySpeed;

	bullet->dimensions.x = round(bullet->xPos);
	bullet->dimensions.y = round(bullet->yPos);

	if (bullet->dimensions.x < 0 ||
		bullet->dimensions.x > WINDOWWIDTH ||
		bullet->dimensions.y < 0 ||
		bullet->dimensions.y > WINDOWHEIGHT) bullet->active = false;

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