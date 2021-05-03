#ifndef NETWORKBULLET_H
#define NETWORKBULLET_H

#include "bullet.h"

// typedef struct Networkbullet_type *Networkbullet;
typedef struct Networkbullet_type {
	bool active;
	float xPos;
	float yPos;
	float xSpeed;
	float ySpeed;
	short owner;
}Networkbullet;

Networkbullet createNetworkbullet();
// void setNetworkbullet(Networkbullet *a, Bullet b);
// int getNetworkbulletX(Networkbullet a);
// int getNetworkbulletY(Networkbullet a);

#endif