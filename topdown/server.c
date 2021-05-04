#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include <stdbool.h>
#include "networkgamestate.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define SERVER_REFRESH_RATE 30  // 30 = 10/s    10 = 30/s
PRIVATE void playerBulletCollisionCheck(Bullet bullets[], Player players[]);


struct Server_type {
    UDPsocket sd;       /* Socket descriptor */
    UDPpacket* pRecive;       /* Pointer to packet memory */
    UDPpacket* pSent;
    TCPsocket tcpsockServer;
    TCPsocket tcpsockClient[MAX_PLAYERS + 1];
    IPaddress serverIP;
    IPaddress clients[MAX_PLAYERS];
    int noOfPlayers;
    Networkgamestate state;
    SDL_Thread* UDPreceiveThread;
    SDL_Thread* UDPsendThread;
    Player aPlayers[MAX_PLAYERS];
    Bullet aBullet[MAX_BULLETS];
    SDL_Point spawnPoint[MAX_PLAYERS];
};

PUBLIC Server createServer()
{
    Server server = malloc(sizeof(struct Server_type));
    server->state = createNetworkgamestate();
    setGamastateplayerpos(&server->state, 0, 1, 1);
    setGamastateplayerpos(&server->state, 1, WINDOWWIDTH - 1, 1);
    setGamastateplayerpos(&server->state, 2, 1, WINDOWHEIGHT - 1);
    setGamastateplayerpos(&server->state, 3, WINDOWWIDTH - 1, WINDOWHEIGHT - 1);
    setGamastateplayerpos(&server->state, 4, 500, 500);
    for (int i = 0; i < MAX_PLAYERS + 1; i++)
    {
        server->tcpsockClient[i] = NULL;
    }

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        server->clients[i].host = 0;
        server->clients[i].port = 0;
        server->aPlayers[i] = createPlayer(getNetworkgamestateplayerX(&server->state, i), getNetworkgamestateplayerY(&server->state, i), i);
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        server->aBullet[i] = createBullet();
    }
    
    server->noOfPlayers = 0;

    // Hårdkodade startpositioner för spelare
    server->spawnPoint[0].x = 1;
    server->spawnPoint[0].y = 1;
    server->spawnPoint[1].x = WINDOWWIDTH - 1;
    server->spawnPoint[1].y = 1;
    server->spawnPoint[2].x = 1;
    server->spawnPoint[2].y = WINDOWHEIGHT - 1;
    server->spawnPoint[3].x = WINDOWWIDTH - 1;
    server->spawnPoint[3].y = WINDOWHEIGHT - 1;
    server->spawnPoint[4].x = 500;
    server->spawnPoint[4].y = 500;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        setGamastateplayerpos(&server->state, i, server->spawnPoint[i].x, server->spawnPoint[i].y);
        snapPlayer(server->aPlayers[i],server->spawnPoint[i].x, server->spawnPoint[i].y);
    }

    //Initialize SDL_net 
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (SDLNet_ResolveHost(&(server->serverIP), NULL, 2000) < 0) 
    { 
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (!(server->tcpsockServer = SDLNet_TCP_Open(&(server->serverIP))))
    { 
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE); 
    } 

    // Open a socket 
    if (!(server->sd = SDLNet_UDP_Open(2000)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    // Make space for the packet 
    if (!((server->pSent = SDLNet_AllocPacket(512)) && (server->pRecive = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    return server;
}

PUBLIC void startServer(Server server)
{
    int TCPdelay = 0;
    int UDPsendDelay = 0;
    int UDPreceiveDelay = 0;
    int Gamelogicdelay = 0;
    int RespawnTimer = 0;
    int delay[MAX_PLAYERS] = {0}; 
    // Main-loop
    while (true)
    {
        SDL_Delay(1); 
        TCPdelay = (TCPdelay + 1) % 1000;
        UDPreceiveDelay = (UDPreceiveDelay + 1) % 1;
        UDPsendDelay = (UDPsendDelay + 1) % 10;
        Gamelogicdelay = (Gamelogicdelay + 1) % 16;
        RespawnTimer = (RespawnTimer + 1) % 3000;

        if(!Gamelogicdelay)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                if(isPlayershooting(server->aPlayers[i]))
                {
                    fire(server->aBullet, &server->aPlayers[i], i, getPlayerxtarget(server->aPlayers[i]), getPlayerytarget(server->aPlayers[i]));
                }
            }
            playerBulletCollisionCheck(server->aBullet, server->aPlayers);
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                if(!isPlayerAlive(server->aPlayers[i]) && server->tcpsockClient[i] != NULL)
                {
                    delay[i]++;
                    snapPlayer(server->aPlayers[i], server->spawnPoint[i].x, server->spawnPoint[i].y);
                    if(delay[i] == 180)
                    {
                        setPlayerAlive(server->aPlayers[i], true);
                        delay[i]=0;
                    }
                }
            }
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                setNetworkgamestateplayer(&server->state, i, server->aPlayers[i]);
            }
        }
        

        // TCP
        if (!TCPdelay)
        {
            // Gå igenom alla TCPsockets för klienter (g som index)
            for (int g = 0; g < MAX_PLAYERS + 1; g++) // En socket för varje spelare PLUS en som används för att säga åt en klient om servern är full
            {
                if (server->tcpsockClient[g] == NULL) // Om socketen är NULL så betyder det att den är ledig
                {
                    server->tcpsockClient[g] = SDLNet_TCP_Accept(server->tcpsockServer); // TCP_Accept kollar om det är någon som försöker kopplar upp sig till servern (Serverns TCPsocket, tcpsockServer)

                    if (server->tcpsockClient[g]) // Om det är någon som svarar så är inte tcpsockClient[g] = NULL längre. Den tilldelas till den som har anslutit.
                    {
                        if (server->noOfPlayers >= MAX_PLAYERS) // Om servern är full, skicka tillbaka -1 som säger åt klienten att stänga av programmet
                        {
                            char msg[1024];
                            int len;
                            sprintf(msg, "%d\n", -1);
                            len = strlen(msg) + 1;
                            SDLNet_TCP_Send(server->tcpsockClient[MAX_PLAYERS], msg, len);
                            server->tcpsockClient[MAX_PLAYERS] = NULL;
                            printf("A player tried to connect, but the server was full :( \n");
                        }
                        else // Annars så körs alla startrutiner. Dela ut spelar ID, skicka ut positioner på alla andra spelare etc.
                        {
                            printf("Client %d has connected\n", g);
                            char msg[1024];
                            int len;
                            int newPlayerID;
                            // for (int i = 0; i < MAX_PLAYERS; i++) // Kollar vilken spelarID som är ledig
                            // {
                                // if (server->state.players[i].id == -1)
                                // if(!isNetworkplayeractive(&server->state, i))
                                // {
                                    newPlayerID = g;
                                    printf("%d\n", g);
                                    // server->state.players[i].id = i;
                                    activateNetworkgamestateplayer(&server->state, g);
                                    reviveNetworkgamestateplayer(&server->state, g);
                                    setPlayerAlive(server->aPlayers[g], true);
                                    // server->state.players[i].isAlive = 1;
                            //         break;
                            //     }
                            // }
                            // Tilldela spelarID och skicka startpositionerna för alla spelare
                            SDLNet_TCP_Send(server->tcpsockClient[g], server->state, getGamestatesize());
                            SDLNet_TCP_Send(server->tcpsockClient[g], &newPlayerID, sizeof(newPlayerID));
                            server->clients[g].host = SDLNet_TCP_GetPeerAddress(server->tcpsockClient[g])->host;
                            // Klienten behöver skicka sin lokala UDP-port till servern så servern vet vilken port den ska skicka UDP-paketen till.
                            SDLNet_TCP_Recv(server->tcpsockClient[g], msg, 1024);
                            sscanf((char*)msg, "%hd\n", &(server->clients[g].port));
                            server->noOfPlayers++;
                        }
                    }
                }
                else // Om socketen inte är NULL så betyder det att den är upptagen av någon klient. 
                     // Servern "Pingar" alla uppkopplade spelare. Om TCP_SEND returnerar en int mindre än "len" så betyder det att klienten inte svarar och servern gör en plats ledig.
                {
                    char msg[1024];
                    int len;
                    sprintf(msg, "%d\n", 7);
                    len = strlen(msg) + 1;
                    if (SDLNet_TCP_Send(server->tcpsockClient[g], msg, len) < len)
                    {
                        printf("Client %d has disconnected\n", g);
                        server->tcpsockClient[g] = NULL; 
                        server->clients[g].host = (int)NULL;
                        server->clients[g].port = (int)NULL;
                        setPlayerAlive(server->aPlayers[g], false);
                        // killNetworkgamestateplayer(&server->state, g);
                        freeNetworkgamestateplayer(&server->state, g);
                        snapPlayer(server->aPlayers[g], server->spawnPoint[g].x, server->spawnPoint[g].y);
                        server->noOfPlayers--;
                    }
                }
            }
        }
        // UDP Receive
        if (!UDPreceiveDelay)
        {
            if (SDLNet_UDP_Recv(server->sd, server->pRecive))
            {
                for (int i = 0; i < MAX_PLAYERS; i++)
                {
                    if (server->pRecive->address.port == server->clients[i].port) // Kollar vem paket kom ifrån och uppdaterar endast den spelaren
                    {
                        memcpy(getNetworkgamestateplayer(&server->state, i), server->pRecive->data, getNetworkplayersize());
                        setNetworkplayeralive(&server->state, i, isPlayerAlive(server->aPlayers[i]));
                        updateServerPlayer(&server->aPlayers[i], getNetworkgamestateplayerX(&server->state, i), getNetworkgamestateplayerY(&server->state, i), getNetworkgamestateplayerDirection(&server->state, i), isNetworkgamestateplayerAlive(&server->state, i) , isNetworkgamestateplayerShooting(&server->state, i) ,getNetworkgamestateplayerXtarget(&server->state, i),getNetworkgamestateplayerYtarget(&server->state, i));                    
                        // memcpy(&server->state.players[i], server->pRecive->data, sizeof(server->state.players[i]));
                    }
                }
            }
        }
        if(!UDPsendDelay)
        {
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                for(int j = 0; j < MAX_PLAYERS; j++)
                {
                    if (i != j && (server->tcpsockClient[j] != NULL))
                    {
                        server->pSent->address = server->clients[j];	// Set the destination host 
                        memcpy(server->pSent->data, server->state, getGamestatesize());
                        server->pSent->len = getGamestatesize();
                        SDLNet_UDP_Send(server->sd, -1, server->pSent);
                    }
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

PRIVATE void playerBulletCollisionCheck(Bullet bullets[], Player players[])
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
                    damagePlayer(players[j], getBulletDamage(bullets[j]));
                }
            }
        }
    }
}