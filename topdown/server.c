#pragma warning(disable : 4996)

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
PRIVATE void handleGameLogic(Server server, int respawnDelay[], SDL_Point *a, SDL_Point *b, int invulnerabilityDelay[]);
PRIVATE void handleTCP(Server server, int respawnDelay[]);
PRIVATE void handleUDPreceive(Server server);
PRIVATE void handleUDPsend(Server server);
PRIVATE void startNewGame(Server server, int respawnDelay[]);
PRIVATE bool circleHitDetect(SDL_Point *a, int r0, SDL_Point *b, int r1);

struct Server_type {
    UDPsocket sd;       /* Socket descriptor */
    UDPpacket* pRecive;       /* Pointer to packet memory */
    UDPpacket* pSent;
    TCPsocket tcpsockServer;
    TCPsocket tcpsockClient[MAX_PLAYERS + 1];
    IPaddress serverIP;
    IPaddress clients[MAX_PLAYERS];
    Networkgamestate state;
    SDL_Point spawnPoint[MAX_PLAYERS];
    short noOfPlayers;
    short bulletTimers[MAX_PLAYERS][MAX_BULLETS];
    SDLNet_SocketSet set;
    bool warmup;
    int spawnTime;
    uint32_t wallStates[27];
    uint8_t wallSyncDelay;
};

PUBLIC Server createServer()
{
    srand(time(0));
    Server server = malloc(sizeof(struct Server_type));
    server->state = createNetworkgamestate();
    server->set = SDLNet_AllocSocketSet(MAX_PLAYERS);
    server->warmup = true;
    server->spawnTime = STANDARDSPAWNTIME;
    server->wallSyncDelay = 0;
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

    // Hard coded start positions for players
    server->spawnPoint[0].x = 96;
    server->spawnPoint[0].y = 256;
    server->spawnPoint[1].x = 550;
    server->spawnPoint[1].y = 90;
    server->spawnPoint[2].x = 1056;
    server->spawnPoint[2].y = 256;
    server->spawnPoint[3].x = 256;
    server->spawnPoint[3].y = 672;
    server->spawnPoint[4].x = 864;
    server->spawnPoint[4].y = 672;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        setNetplayerPos(server->state, i, server->spawnPoint[i].x, server->spawnPoint[i].y);
        for(int j = 0; j < MAX_BULLETS; j++)
        {
            server->bulletTimers[i][j] = 0;
        }
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
    int invulnerabilityDelay[MAX_PLAYERS] = {0};
    int respawnDelay[MAX_PLAYERS] = {0}; 
    copyWallState(server->wallStates);

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        copyWallState(getWallState(server->state, i));
    }
    
    SDL_Point a, b;
    // Main loop
    while (true)
    {
        SDL_Delay(1); 
        // Different delays for different functions. The value to the right of the % determines the delay in ms
        TCPdelay = (TCPdelay + 1) % 200;
        UDPreceiveDelay = (UDPreceiveDelay + 1) % 1;
        UDPsendDelay = (UDPsendDelay + 1) % 16;
        Gamelogicdelay = (Gamelogicdelay + 1) % 8;

        // TCP
        if (!TCPdelay)
        {
            handleTCP(server, respawnDelay);
        }
        // UDP Receive
        if (!UDPreceiveDelay)
        {
            handleUDPreceive(server);
        }
        // Game logic
        if(!Gamelogicdelay)
        {
            handleGameLogic(server, respawnDelay, &a, &b, invulnerabilityDelay);           
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
        if (server->tcpsockClient[i] != NULL) // Send packets only to those who are connected
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
            if (server->pRecive->address.port == server->clients[i].port) // Only update the player that sent the packet
            {
                // Temporary store some old values so the client doesnt overwrite them
                bool alive = isNetPlayerAlive(server->state, i);
                int health = getNetplayerHealth(server->state, i);
                int lives = getNetplayerLives(server->state, i);
                int kills = getNetPlayerKills(server->state, i);
                bool invulnerable = isNetplayerInvulnerable(server->state, i);

                // Read packet
                memcpy(getNetPlayer(server->state, i), server->pRecive->data, getNetPlayerSize());
                for(int j = 0; j < MAX_BULLETS; j++)
                {
                    if(netbulletStatus(server->state, i, j))
                    {
                        server->bulletTimers[i][j] = (server->bulletTimers[i][j] + 1) % 10;
                    }
                    if(server->bulletTimers[i][j])
                    {
                        freeNetbullet(server->state, i, j);
                    }
                    else netBulletclearcontrol(server->state, i, j);
                }
                // ...and restore the values
                setNetPlayerKills(server->state, i, kills);
                setNetPlayerAlive(server->state, i, alive);
                setNetplayerHealth(server->state, i, health);
                setNetplayerLives(server->state, i, lives);
                setNetplayerInvulnerable(server->state, i, invulnerable);
            }
        }
    }
}


PRIVATE void handleTCP(Server server, int respawnDelay[])
{
    // Check all TCP sockets (g as index)
    for (int g = 0; g < MAX_PLAYERS + 1; g++) 
    {
        if (server->tcpsockClient[g] == NULL) // The socket is free if it is NULL
        {
            server->tcpsockClient[g] = SDLNet_TCP_Accept(server->tcpsockServer); // See if someone is trying to connect

            if (server->tcpsockClient[g]) // Assign a socket to a connected player
            {
                if (server->noOfPlayers >= MAX_PLAYERS) // Decline connections if the server is full
                {
                    server->tcpsockClient[MAX_PLAYERS] = NULL;
                    printf("%d seconds | A client tried to connect, but the server was full\n", SDL_GetTicks() / 1000);
                }
                else
                {
                    printf("%d seconds | Client %d has connected\n", SDL_GetTicks() / 1000, g);
                
                    activateNetPlayer(server->state, g);
                    if(server->warmup)
                        {
                            reviveNetPlayer(server->state, g);
                        }
                    
                    // Assign playerID and send the gamestate to the client
                    int newPlayerID = g;
                    SDLNet_TCP_Send(server->tcpsockClient[g], server->state, getGamestatesize());
                    SDLNet_TCP_Send(server->tcpsockClient[g], &newPlayerID, sizeof(newPlayerID));
                    server->clients[g].host = SDLNet_TCP_GetPeerAddress(server->tcpsockClient[g])->host;

                    // Stores the local UDP port of the client
                    char msg[1024];
                    SDLNet_TCP_Recv(server->tcpsockClient[g], msg, 1024);
                    sscanf((char*)msg, "%hd\n", &(server->clients[g].port));
                    server->noOfPlayers++;
                    SDLNet_TCP_AddSocket(server->set, server->tcpsockClient[g]);
                }
            }
        }
        else // Handle other TCP packets from the connected players             
        {
            int ping = 0;
            int response = 0;
          
            SDLNet_CheckSockets(server->set, 0);

            // Check if the host has requested something
            if (SDLNet_SocketReady(server->tcpsockClient[g]))
            {
                SDLNet_TCP_Recv(server->tcpsockClient[g], &response, sizeof(response));
                switch (response)
                {
                    case(1):
                        printf("New game!\n");
                        startNewGame(server, respawnDelay);
                        break;
                    case(2):
                        printf("Warmup!\n");
                        server->warmup = true;
                        setRoundState(server->state, 0);
                        for (int i = 0; i < MAX_PLAYERS; i++)
                        {
                            if(isNetPlayerActive(server->state, i)) reviveNetPlayer(server->state, i);
                        }
                        break;
                    default:
                        break;
                }                               
            }

            // Ping players. Free the socket if they are not responding.
            ping = 0;
            if (SDLNet_TCP_Send(server->tcpsockClient[g], &ping, sizeof(ping)) < sizeof(ping))
            {
                printf("%d seconds | Client %d has disconnected\n", SDL_GetTicks() / 1000, g);
                server->tcpsockClient[g] = NULL;
                server->clients[g].host = (int)NULL;
                server->clients[g].port = (int)NULL;
                setNetplayerLives(server->state, g, -1);
                freeNetPlayer(server->state, g);
                killNetPlayer(server->state, g);
                server->noOfPlayers--;
            }
        }
    }
}

PRIVATE void startNewGame(Server server, int respawnDelay[])
{   
    server->warmup = false;
    setRoundState(server->state, 1);
    
    resetTileGridMap();
    server->wallSyncDelay = 200;
    copyWallState(server->wallStates);
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        copyWallState(getWallState(server->state, i));
    }

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (server->tcpsockClient[i] != NULL)
        {
            setNetPlayerKills(server->state, i, 0);
            setNetplayerPos(server->state, i, server->spawnPoint[i].x, server->spawnPoint[i].y);
            killNetPlayer(server->state, i);
            activateNetPlayer(server->state, i);
            setNetplayerLives(server->state, i, START_LIVES);
            respawnDelay[i] = 0;
            for (int j = 0; j < MAX_BULLETS; j++)
            {
                server->bulletTimers[i][j] = 0;
                freeNetbullet(server->state, i, j);
            }
        }
        
    }
}

PRIVATE void handleGameLogic(Server server, int respawnDelay[], SDL_Point *a, SDL_Point *b, int invulnerabilityDelay[])
{
    // Loop for collision detection
    for (int i = 0; i < MAX_PLAYERS; i++)
        if(isNetPlayerActive(server->state, i))
            for(int j = 0; j < MAX_BULLETS; j++)
                if(isNetbulletActive(server->state, i, j))
                    for (int k = 0; k < MAX_PLAYERS; k++)
                        if((i!=k) && isNetPlayerAlive(server->state, k))
                        {
                            if (isNetplayerInvulnerable(server->state, i))
                            {
                                setNetplayerInvulnerable(server->state, i, false);
                            }
                            a->x = getNetPlayerX(server->state, k);
                            a->y = getNetPlayerY(server->state, k);
                            b->x = getNetBulletX(server->state, i, j);
                            b->y = getNetBulletY(server->state, i, j);
                            if(circleHitDetect(a, 20, b, 1))
                            {
                                server->bulletTimers[i][j] = 1;
                                freeNetbullet(server->state, i, j);
                                if (!isNetplayerInvulnerable(server->state, k))
                                {
                                    damageNetplayer(server->state, k, getNetbulletdamage(server->state, i, j), i);
                                }
                                if (!isNetPlayerAlive(server->state, k))
                                {
                                    if (server->warmup) setNetplayerLives(server->state, k, 1);
                                    printf("%d seconds | Player %d killed player %d\n", SDL_GetTicks() / 1000, i, k);
                                    if (getNetplayerLives(server->state, k) <= 0)
                                    {
                                        printf("%d seconds | Player %d is out of lives!\n", SDL_GetTicks() / 1000, k);

                                        // If only one player is alive, end the game
                                        int noOfPlayersAlive = 0;
                                        for (int l = 0; l < MAX_PLAYERS; l++)
                                        {
                                            if (getNetplayerLives(server->state, l) > 0)
                                            {
                                                noOfPlayersAlive++;
                                            }
                                        }
                                        if (noOfPlayersAlive == 1)
                                        {
                                            setRoundState(server->state, 3);
                                            printf("----------------------------------------\n");
                                            printf("%d seconds | Player % d has won!!!\n", SDL_GetTicks() / 1000, i);
                                            printf("----------------------------------------\n");
                                        }
                                    }
                                }
                            }
                        }
    // Loop for respawn
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!isNetPlayerAlive(server->state, i) && isNetPlayerActive(server->state, i) && (getNetplayerLives(server->state, i) > 0))
        {          
            respawnDelay[i]++;
            setNetplayerPos(server->state, i, server->spawnPoint[i].x, server->spawnPoint[i].y);
            if (respawnDelay[i] >= server->spawnTime)
            {
                if (getRoundState(server->state) == 1) setRoundState(server->state, 2);
                printf("%d seconds | Player %d has respawned\n", SDL_GetTicks() / 1000, i);
                reviveNetPlayer(server->state, i);
                setNetplayerInvulnerable(server->state, i, true);                
                respawnDelay[i] = 0;
            }
        }
    }
    // Loop for invincibility
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (isNetplayerInvulnerable(server->state, i))
        {
            invulnerabilityDelay[i]++;
            if (invulnerabilityDelay[i] == 180)
            {
                setNetplayerInvulnerable(server->state, i, false);
                invulnerabilityDelay[i] = 0;
            }
        }
    }
    if(server->wallSyncDelay)
    {
        resetWallStates(server->state, server->wallStates);
        server->wallSyncDelay--;
        createWalls(server->state, true);
    }
    else
    {
        combineWallstates(server->state, server->wallStates);
        createWalls(server->state, false);
    }
}

// Collision detection based on circles instead of rectangles
PRIVATE bool circleHitDetect(SDL_Point *a, int r0, SDL_Point *b, int r1)
{
    return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y)) < (r0 + r1);
}
