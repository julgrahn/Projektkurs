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
            setPlayerAlive(players[playerID], isNetworkplayerAlive(&networkgamestate, playerID));
            if (!isPlayerAlive(players[playerID]))
            {
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
                    bulletActivate(*((bullets+i*MAX_BULLETS) + j));
                    setBulletXY(*((bullets+i*MAX_BULLETS) + j), getNetbulletX(networkgamestate, i, j), getNetbulletY(networkgamestate, i, j));
                    setBulletSpeed(*((bullets+i*MAX_BULLETS) + j), getNetbulletspeedX(networkgamestate, i, j), getNetbulletspeedY(networkgamestate, i, j));
                }
            }
        }
    }
    
}

// PUBLIC void updateplayerbullets(Networkgamestate networkgamestate, int playerID, Bullet bullets[][MAX_BULLETS])
// {
//     for (int i = 0; i < MAX_BULLETS; i++)
//     {
//         if(isBulletActive(bullets[i]) && netbulletStatus(networkgamestate, playerID, i))
//         {
//             if(!isNetbulletActive(networkgamestate, playerID, i))
//             {
//                 // printf("freebullet\n");
//                 freeBullet(bullets[i]);
//                 // bulletTimer(bullets[i]);
//             }
//         }
//     }
// }

// PUBLIC void updateplayerbullets(Networkgamestate networkgamestate, int playerID, Bullet bullets[])
// {
//     for (int i = 0; i < MAX_BULLETS; i++)
//     {
//         if(isBulletActive(bullets[i]) && netbulletStatus(networkgamestate, playerID, i))
//         {
//             if(!isNetbulletActive(networkgamestate, playerID, i))
//             {
//                 // printf("freebullet\n");
//                 freeBullet(bullets[i]);
//                 // bulletTimer(bullets[i]);
//             }
//         }
//     }
// }

// PUBLIC void playerBulletCollisionCheck(Bullet bullets[], Player players[])
// {
//     for (int i = 0; i < MAX_BULLETS; i++)
//     {
//         if (isBulletActive(bullets[i]))
//         {
//             moveBullet(bullets[i]);
//             // for (int j = 0; j < MAX_PLAYERS; j++)
//             // {
//             //     if (rectCollisionTest(getBulletRect(bullets[i]), getPlayerRect(players[j]))
//             //         && (getBulletOwner(bullets[i]) != j) && isPlayerAlive(players[j]))
//             //     {
//             //         freeBullet(bullets[i]);
//             //     }
//             // }
//         }
//     }
// }


PUBLIC bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b)
{
    if ((a->x) > (b->x) && (a->x) < ((b->x) + (b->w)) && (a->y) > (b->y) && (a->y) < ((b->y) + (b->h)))
        return true;
    return false;
}

PUBLIC void fire(Bullet bullets[], Player* p, int playerID, int xTarget, int yTarget)
{
    if(canShoot(*p))
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!isBulletActive(bullets[i]))
            {
                spawnBullet(bullets[i], getPlayerX(*p), getPlayerY(*p), xTarget, yTarget, playerID);
                break;
            }
        }
    }
}