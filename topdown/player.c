#include <stdlib.h>
#include "player.h"
#include <SDL.h>
//#include <SDL2/SDL.h>

#define WINDOWWIDTH 704
#define WINDOWHEIGTH 704

#define PUBLIC
#define SPEED 10

struct Player_type {
    int health;
    int speed;
    SDL_Rect pDimensions;
};

PUBLIC Player createPlayer(int x, int y)
{
    Player a = malloc(sizeof(struct Player_type));
    a->health = 100;
    a->speed = SPEED;
<<<<<<< Updated upstream

    a->pDimensions.x = x;
    a->pDimensions.y = y;
=======
    a->diaSpeed = sqrt(SPEED * SPEED / 2);
    a->frame = 0;
    a->isMoving = 0;
    a->frameCounter = 0;
    a->direction = 0;
    a->posX = (WINDOWWIDTH - 64) / 2;
    a->posY = (WINDOWHEIGHT - 64) / 2;
    a->pDimensions.x = (WINDOWWIDTH - 64) / 2;
    a->pDimensions.y = (WINDOWHEIGHT - 64) / 2;
>>>>>>> Stashed changes
    a->pDimensions.w = 64;
    a->pDimensions.h = 64;

    return a;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left)
{
<<<<<<< Updated upstream
    if (up && !down) p->pDimensions.y -= SPEED;
    if (down && !up) p->pDimensions.y += SPEED;
    if (left && !right) p->pDimensions.x -= SPEED;
    if (right && !left) p->pDimensions.x += SPEED;

    // Collision detection with window
    if (p->pDimensions.y <= 0) p->pDimensions.y = 0;
    if (p->pDimensions.y >= WINDOWHEIGTH - p->pDimensions.w) p->pDimensions.y = WINDOWHEIGTH - p->pDimensions.w;
    if (p->pDimensions.x <= 0) p->pDimensions.x = 0;
    if (p->pDimensions.x >= WINDOWWIDTH - p->pDimensions.h) p->pDimensions.x = WINDOWWIDTH - p->pDimensions.h;
=======
    double newX = 0, newY = 0;
    p->isMoving = 0;
    if (up && !down) { newY -= p->speed; p->isMoving = 1; }
    if (down && !up) { newY += p->speed; p->isMoving = 1; }
    if (left && !right) { newX -= p->speed; p->isMoving = 1; }
    if (right && !left) { newX += p->speed; p->isMoving = 1; }
    // Calc player absolute pos accounting for diagonal movement scaling
    p->posX += (p->diaSpeed * (newX > 0) + p->diaSpeed * (newX < 0) * (-1)) * (newX != 0 && newY != 0) + newX * !(newX != 0 && newY != 0);
    p->posY += (p->diaSpeed * (newY > 0) + p->diaSpeed * (newY < 0) * (-1)) * (newX != 0 && newY != 0) + newY * !(newX != 0 && newY != 0);

    // printf("%.2f %.2f %.2f %.2f\n", p->posX, p->posY, p->speed, p->diaSpeed);
    // Set new pixel pos of player
    p->pDimensions.x = round(p->posX);
    p->pDimensions.y = round(p->posY);

    // Update player sprite frame
    p->frameCounter = (p->frameCounter + p->isMoving) % (ANIMATIONSPEED + 1);
    p->frame = (p->frame + ((p->frameCounter / ANIMATIONSPEED) * p->isMoving)) % 4;
    // Rotate player
    p->direction = (atan2(mouseY - p->pDimensions.y - 34, mouseX - p->pDimensions.x - 18) * 180 / M_PI) - 6;

    // Collision detection with window
    if (p->pDimensions.y <= 0) p->pDimensions.y = p->posY = 0;
    if (p->pDimensions.y >= WINDOWHEIGHT - p->pDimensions.h) p->pDimensions.y = p->posY = WINDOWHEIGHT - p->pDimensions.h;
    if (p->pDimensions.x <= 0) p->pDimensions.x = p->posX = 0;
    if (p->pDimensions.x >= WINDOWWIDTH - p->pDimensions.w) p->pDimensions.x = p->posX = WINDOWWIDTH - p->pDimensions.w;

  

}

PUBLIC int getPlayerDirection(Player p)
{
    return p->direction;
}

PUBLIC void playerHealth(Player p, int health)
{
>>>>>>> Stashed changes

}

PUBLIC SDL_Rect getPlayerRect(Player p)
{
<<<<<<< Updated upstream
    return p->pDimensions;
}
=======
    return &p->pDimensions;
}

PUBLIC int getPlayerX(Player p)
{
    return p->pDimensions.x;
}

PUBLIC int getPlayerY(Player p)
{
    return p->pDimensions.y;
}

/*PUBLIC void shoot(Player p, Bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!isBulletActive(bullets[i]))
        {
            spawnBullet(p, bullets[i]);
            return;
        }
    }
}*/
>>>>>>> Stashed changes
