#include "gameFunctions.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void updateplayers(Networkgamestate networkgamestate, Player players[], int playerID)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (i != playerID)
        {
            updatePlayerPosition(&players[i], getNetworkgamestateplayerX(&networkgamestate, i), getNetworkgamestateplayerY(&networkgamestate, i), getNetworkgamestateplayerDirection(&networkgamestate, i), isNetworkplayerAlive(&networkgamestate, i));
        }
        else
        {
            setPlayerLives(players[playerID], getNetplayerLives(networkgamestate, playerID));
            setPlayerAlive(players[playerID], isNetworkplayerAlive(&networkgamestate, playerID));
            setPlayerhealth(players[playerID], getNetplayerHealth(networkgamestate, playerID));
            if (!isPlayerAlive(players[playerID]))
            {
                resetPlayer(players[playerID]);
                snapPlayer(players[playerID], getNetworkgamestateplayerX(&networkgamestate, playerID), getNetworkgamestateplayerY(&networkgamestate, playerID));
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
                if(!isNetbulletActive(networkgamestate, i, j) && isBulletActive(*((bullets+i*MAX_BULLETS) + j)))
                {
                    freeBullet(*((bullets+i*MAX_BULLETS) + j));
                }
                else if(isNetbulletActive(networkgamestate, i, j) && !isBulletActive(*((bullets+i*MAX_BULLETS) + j)))
                {
                    spawnBullet2(*((bullets+i*MAX_BULLETS) + j), getNetbulletX(networkgamestate, i, j), getNetbulletY(networkgamestate, i, j), getNetbulletAngle(networkgamestate, i, j));
                    // bulletActivate(*((bullets+i*MAX_BULLETS) + j));
                    // setBulletXY(*((bullets+i*MAX_BULLETS) + j), getNetbulletX(networkgamestate, i, j), getNetbulletY(networkgamestate, i, j));
                    // setBulletSpeed(*((bullets+i*MAX_BULLETS) + j), getNetbulletspeedX(networkgamestate, i, j), getNetbulletspeedY(networkgamestate, i, j));
                    // clientDamagePlayer(players[j]);
                    // freeBullet(bullets[i]);
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

PUBLIC void fire2(Bullet bullets[], Player p, int playerID)
{
    if(canShoot(p))
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!isBulletActive(bullets[i]))
            {
                spawnBullet2(bullets[i], getPlayerGunbarrelX(p), getPlayerGunbarrelY(p), getPlayerDirection(p));
                break;
            }
        }
    }
}

PUBLIC void fire(Bullet bullets[], Player p, int playerID, int xTarget, int yTarget)
{
    if(canShoot(p))
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!isBulletActive(bullets[i]))
            {
                spawnBullet3(bullets[i], getPlayerGunbarrelX(p)-2, getPlayerGunbarrelY(p)-2, getPlayershotangle(p), getPlayerWeapondamage(p));
                // spawnBullet(bullets[i], getPlayerGunbarrelX(p), getPlayerGunbarrelY(p), xTarget, yTarget, playerID, getPlayerWeapondamage(p));
                break;
            }
        }
    }
}