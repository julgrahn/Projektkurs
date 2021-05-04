#include "networkplayer.h"
#include "stdbool.h"
// #include "player.h"
// #include "bullet.h"

#define PUBLIC
#define SPEED 2
#define ANIMATIONSPEED 8               //lower = faster
#define HEALTH 100
#define ROTATION_UPDATE_SPEED 5

// struct Networkplayer_type{
//     short id, health, direction;
//     float speed, posX, posY;
//     bool active;
//     Networkbullet bullets[10];
// };

PUBLIC Networkplayer createNetworkplayer(int id)
{
    Networkplayer a;
    // for(int i = 0; i < 10; i++)
    // {
    //     a.bullets[i] = createNetworkbullet();
    // }
    // a.health = HEALTH;
    // a.speed = SPEED;
    a.direction = 0;
    a.posX = 0, a.posY = 0;
    a.isAlive = false;
    a.isShooting = false;
    a.id = id;
    a.xTarget = 0, a.yTarget = 0;
    return a;
}

// PUBLIC Networkplayer createNetworkplayer(int id)
// {
//     Networkplayer a = malloc(sizeof(struct Networkplayer_type));
//     for(int i = 0; i < 10; i++)
//     {
//         a->bullets[i] = createNetworkbullet();
//     }
//     a->health = HEALTH;
//     a->speed = SPEED;
//     a->direction = 0;
//     a->posX = 200;
//     a->posY = 200;
//     a->active = false;
//     a->id = id;
//     return a;
// }

PUBLIC void copyPlayer(Networkplayer *dest, Networkplayer source)
{
    *dest = source;
}

PUBLIC int getNetplayerX(Networkplayer a)
{
    return a.posX;
}

// PUBLIC unsigned long getNetplayersize()
// {
//     Networkplayer a;
//     return sizeof(*a);
// }

// PUBLIC void setNetplayerX(Networkplayer *a, int x)
// {
//     (*a)->posX = x;
// }

PUBLIC void setNetworkplayer(Networkplayer *a, Player b)
{
    a->isAlive = isPlayerAlive(b);
    a->posX = getPlayerX(b);
    a->posY = getPlayerY(b);
    a->direction = (short)getPlayerDirection(b);
    a->xTarget = getPlayerxtarget(b);
    a->yTarget = getPlayerytarget(b);
    a->isShooting = isPlayershooting(b);
    // for(int i = 0; i < 10; i++)
    // {
    //     setNetworkbullet(&a->bullets[i], c[i]);
    //     // (*a)->bullets[i] = c[i];
    // }
    // (*a)-
}

// PUBLIC Networkbullet getNetworkbullet(Networkplayer *a, int n)
// {
//     return (*a)->bullets[n];
// }