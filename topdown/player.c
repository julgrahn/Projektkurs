#include <stdlib.h>
#include "player.h"
#include <math.h>
// #include <SDL.h>
// #include <SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOWWIDTH 704
#define WINDOWHEIGTH 704

#define PUBLIC
#define SPEED 2
#define ANIMATIONSPEED 15               //lägre värde = snabbare
#define HEALTH 100

struct Player_type {
    SDL_Rect pDimensions;

    int health;
    int speed;
    int frame;
    int frameCounter;
    int isMoving;
    double direction;
};

//int SDL_RenderDrawRect(SDL_Renderer* renderer, SDL_Rect NULL);

PUBLIC Player createPlayer(int x, int y)
{
    Player a = malloc(sizeof(struct Player_type));

    a->health = HEALTH;
    a->speed = SPEED;
    a->frame = 0;
    a->isMoving = 0;
    a->frameCounter = 0;
    a->direction = 0;

    a->pDimensions.x = (WINDOWWIDTH - 64) / 2;
    a->pDimensions.y = (WINDOWHEIGTH - 64) / 2;
    a->pDimensions.w = 64;
    a->pDimensions.h = 64;

    return a;
}

PUBLIC int getPlayerFrame(Player p)
{
    return p->frame;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY)
{
    p->isMoving=0;
    if (up && !down) {p->pDimensions.y -= SPEED; p->isMoving=1;}
    if (down && !up) {p->pDimensions.y += SPEED; p->isMoving=1;}
    if (left && !right) {p->pDimensions.x -= SPEED; p->isMoving=1;}
    if (right && !left) {p->pDimensions.x += SPEED; p->isMoving=1;}

    // Update player sprite frame
    p->frameCounter = (p->frameCounter+p->isMoving)%(ANIMATIONSPEED+1);
    p->frame = (p->frame+((p->frameCounter/ANIMATIONSPEED)*p->isMoving))%4;
    // Rotate player
    p->direction = atan2(mouseY-p->pDimensions.y-32, mouseX-p->pDimensions.x-32)*180/M_PI;
    
    // Collision detection with window
    if (p-> pDimensions.y <= 0 ) p->pDimensions.y = 0;
    if (p-> pDimensions.y >= WINDOWHEIGTH-p->pDimensions.w ) p->pDimensions.y = WINDOWHEIGTH-p->pDimensions.w;
    if (p-> pDimensions.x <= 0 ) p->pDimensions.x = 0;
    if (p-> pDimensions.x >= WINDOWWIDTH-p->pDimensions.h ) p->pDimensions.x = WINDOWWIDTH-p->pDimensions.h;

}

PUBLIC int getPlayerDirection(Player p)
{
    return p->direction;
}

PUBLIC void playerHealth(Player p, int health)
{

}

PUBLIC SDL_Rect* getPlayerRect(Player p)
{
    return &p->pDimensions;
}