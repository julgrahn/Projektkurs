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

PUBLIC void handleMenu(int* mouseX, int* mouseY, bool shooting, Button buttons[], Bullet bullets[], IPaddress* srvadd, TCPsocket* tcpsock, Networkgamestate networkgamestate, int* playerID,
    Player players[], UDPsocket* sd, bool* connected, SDL_mutex* mutex, bool* isPlaying, UDPpacket** p2, Server server, bool* host)
{
    // Connect button
    if (mouseX >= (WINDOWWIDTH / 2) - BUTTON_HEIGHT && mouseX <= (WINDOWWIDTH / 2) + BUTTON_HEIGHT)
    {
        // Connect button

        if (mouseY > CONNECT_Y_POS && mouseY < CONNECT_Y_POS + BUTTON_HEIGHT && shooting)
        {
            setButtonPressed(buttons[0]);
            connectToServer(LOCAL_IP, srvadd, tcpsock, networkgamestate, playerID, players, sd, connected);
            startUDPreceiveThread(sd, p2, bullets, players, &networkgamestate, playerID, &mutex);
            //printf("Connect clicked\n"); 
        }

        // Host button

        if (mouseY > HOST_Y_POS && mouseY < HOST_Y_POS + BUTTON_HEIGHT && shooting)
        {
            server = createServer();
            startServer(server);
            *host = true;
            setButtonPressed(buttons[1]);
            connectToServer(LOCAL_IP, srvadd, tcpsock, networkgamestate, playerID, players, sd, connected);
            startUDPreceiveThread(sd, p2, bullets, players, &networkgamestate, playerID, &mutex);


            //printf("Host clicked\n");
        }

        // Quit button

        if (mouseY > QUIT_Y_POS && mouseY < QUIT_Y_POS + BUTTON_HEIGHT && shooting)
        {

            setButtonPressed(buttons[2], true);
            *isPlaying = false;
        }
    }
}

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