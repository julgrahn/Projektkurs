#include "sdlinclude.h"
#include <stdbool.h>
#include "networkbullet.h"

#define PUBLIC

// struct Networkbullet_type {
// 	bool active;
// 	float xPos;
// 	float yPos;
// 	float xSpeed;
// 	float ySpeed;
// 	short owner;
// };

PUBLIC Networkbullet createNetworkbullet()
{
	Networkbullet a;
	a.active = false;
	// sizeof(*a);
	return a;
}

// PUBLIC void setNetworkbullet(Networkbullet *a, Bullet b)
// {
// 	(*a)->xPos = getBulletX(b);
// 	(*a)->yPos = getBulletY(b);
// }

// PUBLIC int getNetworkbulletX(Networkbullet a)
// {
// 	return a->xPos;
// }

// PUBLIC int getNetworkbulletY(Networkbullet a)
// {
// 	return a->yPos;
// }