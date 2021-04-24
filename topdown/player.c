#include <stdlib.h>
#include "player.h"
#include <math.h>

#define PUBLIC
#define SPEED 2
#define ANIMATIONSPEED 8               //lower = faster
#define HEALTH 100
#define WINDOWHEIGTH 704
#define WINDOWWIDTH 704

// struct Player_type {
//     int health;
//     double speed;
//     double diaSpeed;
//     double posX, posY;
//     SDL_Rect pDimensions;
//     int frame;
//     int frameCounter;
//     int isMoving;
//     double direction;
// };

//int SDL_RenderDrawRect(SDL_Renderer* renderer, SDL_Rect NULL);

PUBLIC Player createPlayer(int x, int y)
{
    Player a = malloc(sizeof(struct Player_type));
    a->health = HEALTH;
    a->speed = SPEED;
    a->diaSpeed = sqrt(SPEED*SPEED/2);    
    a->frame = 0;
    a->isMoving = 0;
    a->frameCounter = 0;
    a->direction = 0;
    a->posX = (WINDOWWIDTH - 64) / 2;
    a->posY = (WINDOWHEIGTH - 64) / 2;
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
    double newX = 0, newY = 0;
    p->isMoving=0;
    if (up && !down) {newY -= p->speed;p->isMoving=1;}
    if (down && !up) {newY += p->speed;p->isMoving=1;}
    if (left && !right) {newX -= p->speed;p->isMoving=1;}
    if (right && !left) {newX += p->speed;p->isMoving=1;}
    // Calc player absolute pos accounting for diagonal movement scaling
    p->posX += (p->diaSpeed*(newX>0) + p->diaSpeed * (newX<0)*(-1))*(newX!=0 && newY!=0) + newX*!(newX!=0 && newY!=0);
    p->posY += (p->diaSpeed*(newY>0) + p->diaSpeed * (newY<0)*(-1))*(newX!=0 && newY!=0) + newY*!(newX!=0 && newY!=0);

    // printf("%.2f %.2f %.2f %.2f\n", p->posX, p->posY, p->speed, p->diaSpeed);
    // Set new pixel pos of player
    p->pDimensions.x = round(p->posX);
    p->pDimensions.y = round(p->posY);

    /* OLD movement. Does not account for diagonal movement scaling
    if (up && !down) {p->pDimensions.y -= SPEED; p->isMoving=1;}
    if (down && !up) {p->pDimensions.y += SPEED; p->isMoving=1;}
    if (left && !right) {p->pDimensions.x -= SPEED; p->isMoving=1;}
    if (right && !left) {p->pDimensions.x += SPEED; p->isMoving=1;} */

    // Update player sprite frame
    p->frameCounter = (p->frameCounter+p->isMoving)%(ANIMATIONSPEED+1);
    p->frame = (p->frame+((p->frameCounter/ANIMATIONSPEED)*p->isMoving))%4;
    // Rotate player
    p->direction = atan2(mouseY-p->pDimensions.y-34, mouseX-p->pDimensions.x-18)*180/M_PI;
    
    // Collision detection with window
    if (p->pDimensions.y <= 0) p->pDimensions.y = p->posY = 0;
    if (p->pDimensions.y >= WINDOWHEIGHT-p->pDimensions.h) p->pDimensions.y = p->posY = WINDOWHEIGHT-p->pDimensions.h;
    if (p->pDimensions.x <= 0) p->pDimensions.x = p->posX = 0;
    if (p->pDimensions.x >= WINDOWWIDTH-p->pDimensions.w) p->pDimensions.x = p->posX = WINDOWWIDTH-p->pDimensions.w;

    /* OLD Collision detection with window
    if (p-> pDimensions.y <= 0 ) p->pDimensions.y = 0;
    if (p-> pDimensions.y >= WINDOWHEIGTH-p->pDimensions.w ) p->pDimensions.y = WINDOWHEIGTH-p->pDimensions.w;
    if (p-> pDimensions.x <= 0 ) p->pDimensions.x = 0;
    if (p-> pDimensions.x >= WINDOWWIDTH-p->pDimensions.h ) p->pDimensions.x = WINDOWWIDTH-p->pDimensions.h; */

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