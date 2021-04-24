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

PUBLIC void spawnBullet(Bullet bullet, int x, int y, int direction)
{
	bullet->active = 1;
	bullet->xPos = x + 20;
	bullet->yPos = y + 32;
	bullet->direction = direction * M_PI / 180;

	//printf("xSpeed: %d\n", bullet->xSpeed);
	//printf("ySpeed: %d\n", bullet->ySpeed);

	bullet->xSpeed = bullet->speed * cos(bullet->direction);
	bullet->ySpeed = bullet->speed * sin(bullet->direction);

}

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

SDL_Rect* getBulletRect(Bullet bullet)
{
	return &bullet->dimensions;
}

void freeBullet(Bullet a)
{
	a->active = false;
}