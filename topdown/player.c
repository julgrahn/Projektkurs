#include <stdlib.h>
#include "player.h"
#include <SDL.h>
//#include <SDL2/SDL.h>

#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720

#define PUBLIC
#define SPEED 10
#define HEALTH 100

struct Player_type {
    int health;
    int speed;
    SDL_Rect pDimensions;
};

PUBLIC Player createPlayer(int x, int y)
{
    Player a = malloc(sizeof(struct Player_type));

    a->health = HEALTH;
    a->speed = SPEED;

    a->pDimensions.x = (WINDOWWIDTH - 64) / 2;
    a->pDimensions.y = (WINDOWHEIGHT - 64) / 2;
    a->pDimensions.w = 64;
    a->pDimensions.h = 64;

    return a;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left)
{
    if (up) p->pDimensions.y -= SPEED;
    if (down) p->pDimensions.y += SPEED;
    if (left) p->pDimensions.x -= SPEED;
    if (right) p->pDimensions.x += SPEED;
    
    // Collision detection with window
    if (p->pDimensions.y <= 0 ) p->pDimensions.y = 0;
    if (p->pDimensions.y >= WINDOWHEIGHT-p->pDimensions.w ) p->pDimensions.y = WINDOWHEIGHT-p->pDimensions.w;
    if (p->pDimensions.x <=0 ) p->pDimensions.x = 0;
    if (p->pDimensions.x >= WINDOWWIDTH-p->pDimensions.h ) p->pDimensions.x = WINDOWWIDTH-p->pDimensions.h;

}

PUBLIC void playerHealth(Player p, int health)
{

}

PUBLIC SDL_Rect getPlayerRect(Player p)
{
    return p->pDimensions;
}