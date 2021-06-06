#include "gameFunctions.h"
#include "button.h"
#include "bullet.h"
#include "server.h"
#include "networkgamestate.h"
#include "clientNetFunctions.h"
#include "player.h"
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static


PUBLIC void updateplayers(Networkgamestate networkgamestate, Player players[], int playerID)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        setActivePlayer(players[i], isNetPlayerActive(networkgamestate, i));
        setPlayerKills(players[i], getNetPlayerKills(networkgamestate, i));
        if (i != playerID)
        {            
            updatePlayerPosition(players[i], getNetPlayerX(networkgamestate, i), getNetPlayerY(networkgamestate, i), getNetPlayerDirection(networkgamestate, i), isNetPlayerAlive(networkgamestate, i));
        }
        else
        {
            updateWallstate(getWallState(networkgamestate, i));
            setPlayerLives(players[playerID], getNetplayerLives(networkgamestate, playerID));
            setPlayerAlive(players[playerID], isNetPlayerAlive(networkgamestate, playerID));
            setPlayerhealth(players[playerID], getNetplayerHealth(networkgamestate, playerID));
            if (!isPlayerAlive(players[playerID]))
            {
                resetPlayer(players[playerID]);
                snapPlayer(players[playerID], getNetPlayerX(networkgamestate, playerID), getNetPlayerY(networkgamestate, playerID));
            }
        }
    }
}

PUBLIC void updateplayerbullets(Networkgamestate networkgamestate, int playerID, Bullet *bullets)//Bullet bullets[][MAX_BULLETS])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if(isBulletActive( *((bullets+playerID*MAX_BULLETS) + i) ) && netbulletStatus(networkgamestate, playerID, i))
        {
            if(!isNetbulletActive(networkgamestate, playerID, i))
            {
                freeBullet(*((bullets+playerID*MAX_BULLETS) + i));
            }
        }
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if(i!=playerID)
        {
            for(int j = 0; j < MAX_BULLETS; j++)
            {
                if(!isNetbulletActive(networkgamestate, i, j) && isBulletActive(*((bullets+i*MAX_BULLETS) + j)) && netbulletStatus(networkgamestate, i, j))
                {
                    freeBullet(*((bullets+i*MAX_BULLETS) + j));
                }
                else if(isNetbulletActive(networkgamestate, i, j) && !isBulletActive(*((bullets+i*MAX_BULLETS) + j)) && !getBulletHitValue(*((bullets+i*MAX_BULLETS) + j)))
                {
                    spawnBullet(*((bullets+i*MAX_BULLETS) + j), getNetBulletX(networkgamestate, i, j), getNetBulletY(networkgamestate, i, j), getNetbulletAngle(networkgamestate, i, j), 0);
                }
            }
        }
    }
}

PUBLIC bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b)
{
    if ((a->x) > (b->x) && (a->x) < ((b->x) + (b->w)) && (a->y) > (b->y) && (a->y) < ((b->y) + (b->h)))
        return true;
    return false;
}

PUBLIC void fire(Bullet bullets[], Player p)
{
    if(canShoot(p))
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!isBulletActive(bullets[i]) && !getBulletHitValue(bullets[i]))
            {
                spawnBullet(bullets[i], getPlayerGunbarrelX(p), getPlayerGunbarrelY(p), getPlayerShotAngle(p), getPlayerWeapondamage(p));            
                return;
            }
        }
    }
}