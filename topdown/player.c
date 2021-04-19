#include <stdlib.h>
#include "player.h"
//#include <SDL.h>
#include <SDL2/SDL.h>

#define WINDOWWIDTH 1280
#define WINDOWHEIGTH 720


#define PUBLIC
#define SPEED 5
#define ANIMATIONSPEED 15               //lägre värde = snabbare

struct Player_type {
    int health;
    int speed;
    SDL_Rect pDimensions;
    int frame;
    int frameCounter;
    int isMoving;
};

PUBLIC Player createPlayer(int x, int y)
{
    Player a = malloc(sizeof(struct Player_type));
    a->health = 100;
    a->speed = SPEED;
    a->frame = 0;
    a->isMoving = 0;
    a->frameCounter = 0;

    a->pDimensions.x = x;
    a->pDimensions.y = y;
    a->pDimensions.w = 64;
    a->pDimensions.h = 64;

    return a;
}

PUBLIC int getPlayerFrame(Player p)
{
    return p->frame;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left)
{
    p->isMoving=0;
    if (up && !down) {p->pDimensions.y -= SPEED; p->isMoving=1;}
    if (down && !up) {p->pDimensions.y += SPEED; p->isMoving=1;}
    if (left && !right) {p->pDimensions.x -= SPEED; p->isMoving=1;}
    if (right && !left) {p->pDimensions.x += SPEED; p->isMoving=1;}

    p->frameCounter = (p->frameCounter+p->isMoving)%(ANIMATIONSPEED+1);     // framecounter går från 0-ANIMATIONSPEED
    p->frame = (p->frame+((p->frameCounter/ANIMATIONSPEED)*p->isMoving))%4; // ökar frame varje gång framcounter == ANIMATIONSPEED

    // Gör samma som ovan
    // if(p->isMoving)
    // {
    //     p->frameCounter++;
    //     if(p->frameCounter==20)
    //     {
    //         p->frameCounter=0;
    //         p->frame++;
    //         if(p->frame==4) p->frame=0;
    //     }
    // }

    // Collision detection with window
    if (p->pDimensions.y <= 0 ) p->pDimensions.y = 0;
    if (p->pDimensions.y >= WINDOWHEIGTH-p->pDimensions.w ) p->pDimensions.y = WINDOWHEIGTH-p->pDimensions.w;
    if (p->pDimensions.x <=0 ) p->pDimensions.x = 0;
    if (p->pDimensions.x >= WINDOWWIDTH-p->pDimensions.h ) p->pDimensions.x = WINDOWWIDTH-p->pDimensions.h;

}

PUBLIC SDL_Rect* getPlayerRect(Player p)
{
    return &p->pDimensions;
}