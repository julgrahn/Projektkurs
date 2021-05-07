#include "gameFunctions.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC void updateplayers(Networkgamestate networkgamestate, Player players[], int playerID)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (i != playerID)
        {
            updatePlayerPosition(&players[i], getNetworkgamestateplayerX(&networkgamestate, i), getNetworkgamestateplayerY(&networkgamestate, i), getNetworkgamestateplayerDirection(&networkgamestate, i), isNetworkgamestateplayerAlive(&networkgamestate, i), isNetworkgamestateplayerShooting(&networkgamestate, i), getNetworkgamestateplayerXtarget(&networkgamestate, i), getNetworkgamestateplayerYtarget(&networkgamestate, i));
        }
        else
        {
            setPlayerAlive(players[playerID], isNetworkgamestateplayerAlive(&networkgamestate, playerID));
            if (!isPlayerAlive(players[playerID]))
            {
                snapPlayer(players[playerID], getNetworkgamestateplayerX(&networkgamestate, playerID), getNetworkgamestateplayerY(&networkgamestate, playerID));
            }
        }
    }
}

PUBLIC void playerBulletCollisionCheck(Bullet bullets[], Player players[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (isBulletActive(bullets[i]))
        {
            moveBullet(bullets[i]);
            for (int j = 0; j < MAX_PLAYERS; j++)
            {
                if (rectCollisionTest(getBulletRect(bullets[i]), getPlayerRect(players[j]))
                    && (getBulletOwner(bullets[i]) != j) && isPlayerAlive(players[j]))
                {
                    freeBullet(bullets[i]);
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

PUBLIC void fire(Bullet bullets[], Player* p, int playerID, int xTarget, int yTarget)
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