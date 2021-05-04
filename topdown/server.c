#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include <stdbool.h>
#include "networkgamestate.h"

#define PUBLIC /* empty */
#define PRIVATE static
#define SERVER_REFRESH_RATE 30  // 30 = 10/s    10 = 30/s
<<<<<<< Updated upstream
=======
PRIVATE void playerBulletCollisionCheck(Bullet bullets[], Player players[]);
PRIVATE void handleGameLogic(Server server, int respawnDelay[]);
PRIVATE void handleTCP(Server server);
PRIVATE void handleUDPreceive(Server server);
PRIVATE void handleUDPsend(Server server);

>>>>>>> Stashed changes

struct Server_type {
    UDPsocket sd;       /* Socket descriptor */
    UDPpacket* pRecive;       /* Pointer to packet memory */
    UDPpacket* pSent;
    TCPsocket tcpsockServer;
    TCPsocket tcpsockClient[MAX_PLAYERS + 1];
    IPaddress serverIP;
    IPaddress clients[MAX_PLAYERS];
    int noOfPlayers;
    int send;
    Networkgamestate state;
    SDL_Thread* UDPreceiveThread;
    SDL_Thread* UDPsendThread;
};

PUBLIC Server createServer()
{

    Server server = malloc(sizeof(struct Server_type));
    server->send = 0;
    server->state = createNetworkgamestate();

    for (int i = 0; i < MAX_PLAYERS + 1; i++)
    {
        server->tcpsockClient[i] = NULL;
    }

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        server->clients[i].host = 0;
        server->clients[i].port = 0;
    }
    server->noOfPlayers = 0;

    // Hårdkodade startpositioner för spelare
    server->state.players[0].posX = 1;
    server->state.players[0].posY = 1;
    server->state.players[1].posX = WINDOWWIDTH - 1;
    server->state.players[1].posY = 1;
    server->state.players[2].posX = 1;
    server->state.players[2].posY = WINDOWHEIGHT - 1;
    server->state.players[3].posX = WINDOWWIDTH - 1;
    server->state.players[3].posX = WINDOWHEIGHT - 1;
    server->state.players[4].posX = 500;
    server->state.players[4].posX = 500;

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
<<<<<<< Updated upstream
    //server->UDPreceiveThread = SDL_CreateThread(UDPReceive, "UDPReceive", server);
    //server->UDPsendThread = SDL_CreateThread(UDPSend, "UDPSend", server);
    static int TCPdelay = 0;
    static int UDPsendDelay = 0;
    static int UDPreceiveDelay = 0;
    // Main-loop
    while (true)
    {
        SDL_Delay(1);
        TCPdelay = (TCPdelay + 1) % 1000;
        UDPreceiveDelay = (UDPreceiveDelay + 1) % 1;
        UDPsendDelay = (UDPsendDelay + 1) % 10;


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
                            for (int i = 0; i < MAX_PLAYERS; i++) // Kollar vilken spelarID som är ledig
                            {
                                if (server->state.players[i].id == -1)
                                {
                                    newPlayerID = i;
                                    server->state.players[i].id = i;
                                    server->state.players[i].isAlive = 1;
                                    break;
                                }
                            }
                            // Tilldela spelarID och skicka startpositionerna för alla spelare
                            SDLNet_TCP_Send(server->tcpsockClient[g], &server->state, sizeof(server->state));
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
                        server->state.players[g].isAlive = 0;
                        server->state.players[g].id = -1;
                        server->noOfPlayers--;
                    }
                }
            }
=======
    int TCPdelay = 0;
    int UDPsendDelay = 0;
    int UDPreceiveDelay = 0;
    int Gamelogicdelay = 0;
    int RespawnTimer = 0;
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
        if(!Gamelogicdelay)
        {
            //printf("%d\n", SDL_GetTicks());
            handleGameLogic(server, respawnDelay);           
        }
        
        // TCP
        if (!TCPdelay)
        {
            handleTCP(server);
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
                for (int i = 0; i < MAX_PLAYERS; i++)
                {
                    if (server->pRecive->address.port == server->clients[i].port) // Kollar vem paket kom ifrån och uppdaterar endast den spelaren
                    {
                        memcpy(&server->state.players[i], server->pRecive->data, sizeof(server->state.players[i]));
                    }
                }
            }
        }
        if (!UDPsendDelay)
=======
                memcpy(getNetworkgamestateplayer(&server->state, i), server->pRecive->data, getNetworkplayersize());
                setNetworkplayeralive(&server->state, i, isPlayerAlive(server->aPlayers[i]));
                updateServerPlayer(&server->aPlayers[i], getNetworkgamestateplayerX(&server->state, i), getNetworkgamestateplayerY(&server->state, i), getNetworkgamestateplayerDirection(&server->state, i), isNetworkgamestateplayerAlive(&server->state, i), isNetworkgamestateplayerShooting(&server->state, i), getNetworkgamestateplayerXtarget(&server->state, i), getNetworkgamestateplayerYtarget(&server->state, i));
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
>>>>>>> Stashed changes
        {
            server->tcpsockClient[g] = SDLNet_TCP_Accept(server->tcpsockServer); // TCP_Accept kollar om det är någon som försöker kopplar upp sig till servern (Serverns TCPsocket, tcpsockServer)

            if (server->tcpsockClient[g]) // Om det är någon som svarar så är inte tcpsockClient[g] = NULL längre. Den tilldelas till den som har anslutit.
            {
<<<<<<< Updated upstream
                for (int j = 0; j < MAX_PLAYERS; j++)
                {
                    if (i != j && (server->tcpsockClient[j] != NULL))
                    {
                        server->pSent->address = server->clients[j];	// Set the destination host 
                        memcpy(server->pSent->data, &server->state, sizeof(server->state));
                        server->pSent->len = sizeof(server->state);
                        SDLNet_UDP_Send(server->sd, -1, server->pSent);
                    }
                }
=======
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
>>>>>>> Stashed changes
            }
        }
    }
}


// Trådar som inte används just nu

<<<<<<< Updated upstream
// PUBLIC static void UDPReceive(void* serverPtr)
// {
//     Server server = (Server)serverPtr;
//     while (true)
//     {      
//         SDL_Delay(1); // Hur ofta servern scannar för att ta emot paket
//         if (SDLNet_UDP_Recv(server->sd, server->pRecive))
//         {          
//             for (int i = 0; i < server->noOfPlayers; i++)
//             {
//                 if (server->pRecive->address.port == server->clients[i].port) // KOllar vem paket kom ifrån och uppdaterar endast den spelaren
//                 {
//                     sscanf((char*)server->pRecive->data, "%d %d %d %d\n", &server->playerX[i], &server->playerY[i], &server->playerID[i], &server->playerRotation[i]);
//                 }
//             }          
//         }        
//     }     
// }

// PUBLIC static void UDPSend(void* serverPtr)
// {
//     Server server = (Server)serverPtr;
//     while (true)
//     {
//         SDL_Delay(100); // Hur ofta servern skickar paket

//         // Spelar i's information skickas till spelare j
//         for (int i = 0; i < server->noOfPlayers; i++)
//         {
//             for (int j = 0; j < server->noOfPlayers; j++)
//             {
//                 if (i != j)
//                 {

//                     server->pSent->address = server->clients[j];	// Set the destination host 
//                     sprintf((char*)server->pSent->data, "%d %d %d %d\n", server->playerX[i], server->playerY[i], server->playerID[i], (int)server->playerRotation[i]);
//                     server->pSent->len = strlen((char*)server->pSent->data) + 1;
//                     SDLNet_UDP_Send(server->sd, -1, server->pSent);                 
//                 }
//             }
//         }
//     }

// }
=======
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
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (isPlayershooting(server->aPlayers[i]))
        {
            fire(server->aBullet, &server->aPlayers[i], i, getPlayerxtarget(server->aPlayers[i]), getPlayerytarget(server->aPlayers[i]));
        }
    }
    playerBulletCollisionCheck(server->aBullet, server->aPlayers);
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!isPlayerAlive(server->aPlayers[i]) && server->tcpsockClient[i] != NULL)
        {
            respawnDelay[i]++;
            snapPlayer(server->aPlayers[i], server->spawnPoint[i].x, server->spawnPoint[i].y);
            if (respawnDelay[i] == 180)
            {
                printf("%d seconds | Player %d has respawned\n", SDL_GetTicks() / 1000, i);
                setPlayerAlive(server->aPlayers[i], true);
                respawnDelay[i] = 0;
            }
        }
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        setNetworkgamestateplayer(&server->state, i, server->aPlayers[i]);
    }
}
>>>>>>> Stashed changes
