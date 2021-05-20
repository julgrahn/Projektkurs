#include <stdlib.h>
#include "player.h"
#include "world.h"
#include "weapon.h"
#include <math.h>

#define PUBLIC
#define SPEED 2
#define ANIMATIONSPEED 8               //lower = faster
#define HEALTH 100
#define ROTATION_UPDATE_SPEED 5
#define SNAP_DISTANCE 10
#define PLAYER_CENTER_OFFSET_X 20
#define PLAYER_CENTER_OFFSET_Y 32

struct Player_type {
    int health;
    double speed;
    double diaSpeed;
    double posX, posY;
    SDL_Rect pDimensions;
    int frame;
    int frameCounter;
    int isMoving;
    double direction;
    bool active;
    bool alive;
    int newX;
    int newY;
    double xSpeed, ySpeed;
    int newDirection;
    Weapon gun;
    int gunBarrelX, gunBarrelY;
    int lives;
    double shotAngle;
    bool killed;
    int killTimer;
};

PUBLIC Player createPlayer(int x, int y)
{
    Player a = malloc(sizeof(struct Player_type));
    a->health = HEALTH;
    a->speed = SPEED;
    a->diaSpeed = sqrt(SPEED * SPEED / 2);
    a->frame = 0;
    a->isMoving = 0;
    a->frameCounter = 0;
    a->direction = 0;
    a->posX = x;
    a->posY = y;
    a->pDimensions.x = x-PLAYER_CENTER_OFFSET_X;
    a->pDimensions.y = y-PLAYER_CENTER_OFFSET_Y;
    a->pDimensions.w = 64;
    a->pDimensions.h = 64;
    a->newX = x;
    a->newY = y;
    a->active = false;
    a->alive = false;
    a->xSpeed = a->ySpeed = 0;
    a->newDirection = 0;
    a->gun = createWeapon();
    a->lives = 0;
    a->killed = false;
    a->killTimer = 5;
    return a;
}

PUBLIC int getPlayerFrame(Player p)
{
    return p->frame;
}

PUBLIC void movePlayer(Player p, int up, int down, int right, int left, int mouseX, int mouseY, bool reload)
{
    if(reload) reloadWeapon(p->gun);
    int newX = 0, newY = 0, diagonal;
    p->isMoving = 0;
    if (up && !down) { newY--; p->isMoving = 1; }
    if (down && !up) { newY++; p->isMoving = 1; }
    if (left && !right) { newX--; p->isMoving = 1; }
    if (right && !left) { newX++; p->isMoving = 1; }
    diagonal = (newX != 0 && newY != 0);
    // Set player absolute pos
    p->posX += p->diaSpeed * diagonal * newX + p->speed * !diagonal * newX;
    p->posY += p->diaSpeed * diagonal * newY + p->speed * !diagonal * newY;

    // Set new pixel pos of player
    p->pDimensions.x = round(p->posX)-PLAYER_CENTER_OFFSET_X;
    p->pDimensions.y = round(p->posY)-PLAYER_CENTER_OFFSET_Y;
    if (getWallCollisionPlayer(p->pDimensions.x, p->pDimensions.y))   // Collision x-led
    {
        if (right)
        {
            p->posX -= 2;
            p->pDimensions.x -= 2;
        }
        if (left)
        {
            p->posX += 2;
            p->pDimensions.x += 2;
        }
    }
    if (getWallCollisionPlayer(p->pDimensions.x, p->pDimensions.y))   // Collision y-led
    {
        if (up)
        {
            p->posY += 2;
            p->pDimensions.y += 2;
        }
        if (down)
        {
            p->posY -= 2;
            p->pDimensions.y -= 2;
        }
    }
    // Update player sprite frame
    p->frameCounter = (p->frameCounter + p->isMoving) % (ANIMATIONSPEED + 1);
    p->frame = (p->frame + ((p->frameCounter / ANIMATIONSPEED) * p->isMoving)) % 4;
    // Rotate player
    p->direction = (atan2(mouseY - p->pDimensions.y - 34, mouseX - p->pDimensions.x - 18) * 180 / M_PI) - 6;

    p->shotAngle = atan2(mouseY - getPlayerGunbarrelY(p), mouseX - getPlayerGunbarrelX(p));
    // Collision detection with window
    if (p->pDimensions.y <= 0) {p->pDimensions.y = 0; p->posY = PLAYER_CENTER_OFFSET_Y;}
    if (p->pDimensions.y >= WINDOWHEIGHT - p->pDimensions.h) { p->pDimensions.y = WINDOWHEIGHT - p->pDimensions.h; p->posY = p->pDimensions.y + PLAYER_CENTER_OFFSET_Y;}
    if (p->pDimensions.x <= 0) {p->pDimensions.x = 0; p->posX = PLAYER_CENTER_OFFSET_X;}
    if (p->pDimensions.x >= WINDOWWIDTH - p->pDimensions.w) { p->pDimensions.x = WINDOWWIDTH - p->pDimensions.w; p->posX = p->pDimensions.x + PLAYER_CENTER_OFFSET_X;}

}

PUBLIC double getPlayerDirection(Player p)
{
    return p->direction;
}

PUBLIC int getPlayerHealth(Player p)
{
    return p->health;
}

PUBLIC SDL_Rect* getPlayerRect(Player p)
{
    return &p->pDimensions;
}

PUBLIC int getPlayerX(Player p)
{
    return p->posX;
}

PUBLIC int getPlayerY(Player p)
{
    return p->posY;
}

PUBLIC void activatePlayer(Player p)
{
    p->active = true;
}

PUBLIC void updatePlayerPosition(Player p, int x, int y, int direction, bool alive)
{
    p->alive = alive;
    p->newX = x;
    p->newY = y;
    p->newDirection = direction;
    p->direction = direction;
}

PUBLIC void moveOtherPlayers(Player p)
{
    int xDelta = p->newX - p->posX;
    int yDelta = p->newY - p->posY;
    double distance = sqrt(xDelta * xDelta + yDelta * yDelta);
    double scaling = p->speed / (distance * (distance >= 1) + (distance < 1));
    if (distance >= SNAP_DISTANCE)
    {
        snapPlayer(p, p->newX, p->newY);
        return;
    }
    if (xDelta > 1 || xDelta < -1 || yDelta > 1 || yDelta < -1)
    {
        p->xSpeed = scaling * xDelta;
        p->ySpeed = scaling * yDelta;

        p->posX += p->xSpeed;
        p->posY += p->ySpeed;

        p->pDimensions.x = round(p->posX)-PLAYER_CENTER_OFFSET_X;
        p->pDimensions.y = round(p->posY)-PLAYER_CENTER_OFFSET_Y;
        p->frameCounter = (p->frameCounter + 1) % (ANIMATIONSPEED + 1);
        p->frame = (p->frame + ((p->frameCounter / ANIMATIONSPEED))) % 4;
    }
}

// PUBLIC void moveOtherPlayers(Player p)
// {
//     int xDelta = p->newX - p->pDimensions.x;
//     int yDelta = p->newY - p->pDimensions.y;
//     double distance = sqrt(xDelta * xDelta + yDelta * yDelta);
//     double scaling = p->speed / (distance * (distance >= 1) + (distance < 1));
//     if (distance >= SNAP_DISTANCE)
//     {
//         snapPlayer(p, p->newX, p->newY);
//         return;
//     }
//     int old = p->direction + 180 + 5;
//     int new = p->newDirection + 180 + 5;

//     if (xDelta > 1 || xDelta < -1 || yDelta > 1 || yDelta < -1)
//     {
//         p->xSpeed = scaling * xDelta;
//         p->ySpeed = scaling * yDelta;

//         p->posX += p->xSpeed;
//         p->posY += p->ySpeed;

//         p->pDimensions.x = round(p->posX);
//         p->pDimensions.y = round(p->posY);
//         p->frameCounter = (p->frameCounter + 1) % (ANIMATIONSPEED + 1);
//         p->frame = (p->frame + ((p->frameCounter / ANIMATIONSPEED))) % 4;
//     }
//     // if(p->direction != p->newDirection)
//     // {
//     //     if(new - old < 180 && new - old > 0)
//     //     {
//     //         old += ROTATION_UPDATE_SPEED;
//     //     }
//     //     else if(new - old > -180 && new - old < 0)
//     //     {
//     //         old -= ROTATION_UPDATE_SPEED;
//     //     }
//     //     else if(new - old < -180)
//     //     {
//     //             old += ROTATION_UPDATE_SPEED;
//     //         if(old > 360)
//     //         {
//     //             old -= 360;
//     //         }
//     //     }
//     //     else if(new - old > 180)
//     //     {
//     //         old -= ROTATION_UPDATE_SPEED;
//     //         if(old < 0)
//     //         {
//     //             old += 360;
//     //         }
//     //         // if(new - old < ROTATION_UPDATE_SPEED) old = new;
//     //     }
//     //     old -= (180+5);
//     //     p->direction = old;
//     // }

// }

PUBLIC void snapPlayer(Player p, int x, int y)
{
    p->pDimensions.x = x-PLAYER_CENTER_OFFSET_X;
    p->pDimensions.y = y-PLAYER_CENTER_OFFSET_Y;
    p->newX = x;
    p->newY = y;
    p->posX = x;
    p->posY = y;
}

PUBLIC void damagePlayer(Player p, int damage)
{
    p->health -= damage;
    if (p->health <= 0)
    {
        p->alive = false;
        p->killed = true; 
    }

}

PUBLIC bool isPlayerAlive(Player p)
{
    return p->alive;
}

PUBLIC void setPlayerAlive(Player p, bool value)
{
    p->health = HEALTH;
    p->alive = value;
}

PUBLIC bool canShoot(Player a)
{
    return fireWeapon(a->gun);
}

PUBLIC void playerTick(Player a)
{
    weaponTick(a->gun);
}

PUBLIC int getPlayerGunbarrelX(Player a)
{
    // return round(a->pDimensions.x + 20 + (34*sin((-a->direction + 72)*M_PI/180)));   
    return round(a->posX + (34*sin((-a->direction + 72)*M_PI/180)));   
}

PUBLIC int getPlayerGunbarrelY(Player a)
{
    // return round(a->pDimensions.y + 32 + (34*cos((-a->direction + 72)*M_PI/180)));
    return round(a->posY + (34*cos((-a->direction + 72)*M_PI/180)));
}

PUBLIC int getPlayerWeapondamage(Player a)
{
    return getWeapondamage(a->gun);
}

PUBLIC int getPlayerweaponMag(Player a)
{
    return getMag(a->gun);
}

PUBLIC void setPlayerhealth(Player a, int health)
{
    a->health = health;
}

PUBLIC void setPlayerLives(Player a, int lives)
{
    a->lives = lives;
}

PUBLIC int getPlayerlives(Player a)
{
    return a->lives;
}

PUBLIC int getPlayerReloadprogress(Player a)
{
    return getReloadprogress(a->gun);
}

PUBLIC void resetPlayer(Player a)
{
    resetWeapon(a->gun);
}

PUBLIC double getPlayerShotAngle(Player a)
{
    return a->shotAngle;
}

PUBLIC bool checkKilled(Player a) // experiment f�r att testa d�dsljud
{
    if (a->killed)
    {
        a->killed = false;
        return true;
    }
    else return false;
}

PUBLIC bool killedTimer(Player a)
{
    a->killTimer-1; 
    if (a->killTimer == 0)
    {
        a->killTimer = 5;
        return false; 
    }
    return true; 
}   

