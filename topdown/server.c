#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include <stdbool.h>
#include "networkgamestate.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define SERVER_REFRESH_RATE 30  // 30 = 10/s    10 = 30//

PRIVATE void runServer(void* args);
PRIVATE void serverPlayerBulletCollisionCheck(Bullet bullets[], Player players[]);
PRIVATE void handleGameLogic(Server server, int respawnDelay[]);
PRIVATE void handleTCP(Server server);
PRIVATE void handleUDPreceive(Server server);
PRIVATE void handleUDPsend(Server server);



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
    if (!((server->pSent = SDLNet_AllocPacket(1024)) && (server->pRecive = SDLNet_AllocPacket(1024))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    return server;
}

PUBLIC void startServer(Server server)
{
    SDL_Thread* serverThread;
    serverThread = SDL_CreateThread((SDL_ThreadFunction)runServer, "TestThread", server);
}

PRIVATE void runServer(void* args)
{
    Server server = (Server)args;

    int TCPdelay = 0;
    int UDPsendDelay = 0;
    int UDPreceiveDelay = 0;
    int Gamelogicdelay = 0;
    int respawnDelay[MAX_PLAYERS] = {0}; 
    // Main-loop
    while (true)
    {   
        SDL_Delay(1); 
        TCPdelay = (TCPdelay + 1) % 1000;
        UDPreceiveDelay = (UDPreceiveDelay + 1) % 1;
        UDPsendDelay = (UDPsendDelay + 1) % 10;
        Gamelogicdelay = (Gamelogicdelay + 1) % 8;
        
        // Game logic
        if(!Gamelogicdelay) // Måste vara 16ms intervall
        {
            //printf("%d\n", SDL_GetTicks());
            handleGameLogic(server, respawnDelay);           
        }
        // TCP
        if (!TCPdelay)
        {
            handleTCP(server);
        }
        // UDP Receive
        if (!UDPreceiveDelay)
        {
            handleUDPreceive(server);
        }
        // UDP Send
        if(!UDPsendDelay)
        {
            handleUDPsend(server);
        }
    }
}

PRIVATE void handleUDPsend(Server server)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (server->tcpsockClient[i] != NULL) // Skicka bara paket till de som är anslutna
        {
            server->pSent->address = server->clients[i];
            memcpy(server->pSent->data, server->state, getGamestatesize());
            server->pSent->len = getGamestatesize();
            SDLNet_UDP_Send(server->sd, -1, server->pSent);
        }
    }
}

PRIVATE void handleUDPreceive(Server server)
{
    if (SDLNet_UDP_Recv(server->sd, server->pRecive))
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (server->pRecive->address.port == server->clients[i].port) // Kollar vem paket kom ifrån och uppdaterar endast den spelaren
            {
                bool alive = getNetplayerHealth(server->state, i);
                int health = isNetworkplayerAlive(&server->state, i);
                memcpy(getNetworkgamestateplayer(&server->state, i), server->pRecive->data, getNetworkplayersize());
                setNetworkplayeralive(&server->state, i, alive);
                setNetplayerHealth(server->state, i, health);
                // setNetworkplayeralive(&server->state, i, isPlayerAlive(server->aPlayers[i]));
                // updateServerPlayer(&server->aPlayers[i], getNetworkgamestateplayerX(&server->state, i), getNetworkgamestateplayerY(&server->state, i), getNetworkgamestateplayerDirection(&server->state, i), isNetworkgamestateplayerAlive(&server->state, i), isNetworkgamestateplayerShooting(&server->state, i), getNetworkgamestateplayerXtarget(&server->state, i), getNetworkgamestateplayerYtarget(&server->state, i));
            }
        }
    }
}

PRIVATE void handleTCP(Server server)
{
    // Gå igenom alla TCPsockets för klienter (g som index)
    for (int g = 0; g < MAX_PLAYERS + 1; g++) // En socket för varje spelare PLUS en som används för att säga åt en klient om servern är full
    {
        if (server->tcpsockClient[g] == NULL) // Om socketen är NULL så betyder det att den är ledig
        {
            server->tcpsockClient[g] = SDLNet_TCP_Accept(server->tcpsockServer); // TCP_Accept kollar om det är någon som försöker kopplar upp sig till servern (Serverns TCPsocket, tcpsockServer)

            if (server->tcpsockClient[g]) // Om det är någon som svarar så är inte tcpsockClient[g] = NULL längre. Den tilldelas till den som har anslutit.
            {
                if (server->noOfPlayers >= MAX_PLAYERS) // Om servern är full, bryt kontakten med den som försökte ansluta
                {
                    // Här kan kod skrivas för att behandla spelare som ansluter när servern är full
                    server->tcpsockClient[MAX_PLAYERS] = NULL;
                    printf("%d seconds | A client tried to connect, but the server was full :(\n", SDL_GetTicks() / 1000);
                }
                else // Annars så körs alla startrutiner. Dela ut spelar ID, skicka ut positioner på alla andra spelare etc.
                {
                    printf("%d seconds | Client %d has connected\n", SDL_GetTicks() / 1000, g);

                    // Alla spelare är döda från början så de behöver ressas                   
                    activateNetworkgamestateplayer(&server->state, g);
                    reviveNetworkgamestateplayer(&server->state, g);
                    setPlayerAlive(server->aPlayers[g], true);

                    // Tilldela spelarID och skicka startpositionerna för alla spelare
                    int newPlayerID = g;
                    SDLNet_TCP_Send(server->tcpsockClient[g], server->state, getGamestatesize());
                    SDLNet_TCP_Send(server->tcpsockClient[g], &newPlayerID, sizeof(newPlayerID));
                    server->clients[g].host = SDLNet_TCP_GetPeerAddress(server->tcpsockClient[g])->host;

                    // Klienten behöver skicka sin lokala UDP-port till servern så servern vet vilken port den ska skicka UDP-paketen till.
                    char msg[1024];
                    SDLNet_TCP_Recv(server->tcpsockClient[g], msg, 1024);
                    sscanf((char*)msg, "%hd\n", &(server->clients[g].port));
                    server->noOfPlayers++;
                }
            }
        }
        else // Om socketen inte är NULL så betyder det att den är upptagen av någon klient. 
             // Servern "Pingar" alla uppkopplade spelare. Om TCP_Send returnerar en int mindre än "sizeof(ping)" så betyder det att klienten inte svarar och servern gör en plats ledig.
        {
            int ping = 0;

            if (SDLNet_TCP_Send(server->tcpsockClient[g], &ping, sizeof(ping)) < sizeof(ping))
            {
                printf("%d seconds | Client %d has disconneted\n", SDL_GetTicks() / 1000, g);
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

PRIVATE void serverPlayerBulletCollisionCheck(Bullet bullets[], Player players[])
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
                    damagePlayer(players[j], getBulletDamage(bullets[i]));
                    if (!isPlayerAlive(players[j]))
                    {
                        printf("%d seconds | Player %d killed player %d\n", SDL_GetTicks() / 1000, getBulletOwner(bullets[i]), j);
                    }
                }
            }
        }
    }
}


PRIVATE void handleGameLogic(Server server, int respawnDelay[])
{
    // serverPlayerBulletCollisionCheck(server->aBullet, server->aPlayers);
    SDL_Rect a, b;
    a.h = a.w = 64;
    b.h = b.w = 4;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if(isNetworkplayeractive(&server->state, i))
        {
            for (int j = 0; j < MAX_BULLETS; j++)
            {
                if(isNetbulletActive(server->state, i, j))
                {
                    for(int k = 0; k < MAX_PLAYERS; k++)
                    {
                        if(i!=k && isNetworkplayerAlive(&server->state, k))
                        {
                            a.x = getNetworkgamestateplayerX(&server->state, k);
                            a.y = getNetworkgamestateplayerY(&server->state, k);
                            b.x = getNetbulletX(server->state, i, j);
                            b.y = getNetbulletY(server->state, i, j);
                            if(rectCollisionTest(&b,&a))
                            {
                                freeNetbullet(server->state, i, j);
                                damageNetplayer(server->state, k);
                                // printf("damage\n");
                            }
                        }
                    }
                }
            } 
        }
    }

    // printf("done\n");
    
    
    // for (int i = 0; i < MAX_PLAYERS; i++)
    // {
    //     if (!isPlayerAlive(server->aPlayers[i]) && server->tcpsockClient[i] != NULL)
    //     {
    //         respawnDelay[i]++;
    //         snapPlayer(server->aPlayers[i], server->spawnPoint[i].x, server->spawnPoint[i].y);
    //         if (respawnDelay[i] == 180)
    //         {
    //             printf("%d seconds | Player %d has respawned\n", SDL_GetTicks() / 1000, i);
    //             setPlayerAlive(server->aPlayers[i], true);
    //             respawnDelay[i] = 0;
    //         }
    //     }
    // }
}

// PRIVATE void handleGameLogic(Server server, int respawnDelay[])
// {
//     for (int i = 0; i < MAX_PLAYERS; i++)
//     {
//         if (isPlayershooting(server->aPlayers[i]))
//         {
//             fire(server->aBullet, &server->aPlayers[i], i, getPlayerxtarget(server->aPlayers[i]), getPlayerytarget(server->aPlayers[i]));
//         }
//     }
//     serverPlayerBulletCollisionCheck(server->aBullet, server->aPlayers);
//     for (int i = 0; i < MAX_PLAYERS; i++)
//     {
//         if (!isPlayerAlive(server->aPlayers[i]) && server->tcpsockClient[i] != NULL)
//         {
//             respawnDelay[i]++;
//             snapPlayer(server->aPlayers[i], server->spawnPoint[i].x, server->spawnPoint[i].y);
//             if (respawnDelay[i] == 180)
//             {
//                 printf("%d seconds | Player %d has respawned\n", SDL_GetTicks() / 1000, i);
//                 setPlayerAlive(server->aPlayers[i], true);
//                 respawnDelay[i] = 0;
//             }
//         }
//     }
//     for (int i = 0; i < MAX_PLAYERS; i++)
//     {
//         setNetworkgamestateplayer(&server->state, i, server->aPlayers[i]);
//     }
// }