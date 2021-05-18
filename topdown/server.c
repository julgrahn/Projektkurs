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
PRIVATE bool circleHitDetect(SDL_Point *a, int rad0, SDL_Point *b, int rad1);
 

struct Server_type {
    UDPsocket sd;       /* Socket descriptor */
    UDPpacket* pRecive;       /* Pointer to packet memory */
    UDPpacket* pSent;
    TCPsocket tcpsockServer;
    TCPsocket tcpsockClient[MAX_PLAYERS + 1];
    IPaddress serverIP;
    IPaddress clients[MAX_PLAYERS];
    Networkgamestate state;
    SDL_Thread* UDPreceiveThread;
    SDL_Thread* UDPsendThread;
    SDL_Point spawnPoint[MAX_PLAYERS];
    short noOfPlayers;
    short bulletTimers[MAX_PLAYERS][MAX_BULLETS];
    SDLNet_SocketSet set;
    bool warmup;
    int spawnTime;
};

PUBLIC Server createServer()
{
    Server server = malloc(sizeof(struct Server_type));
    server->state = createNetworkgamestate();
    server->set = SDLNet_AllocSocketSet(MAX_PLAYERS);
    server->warmup = true;
    server->spawnTime = STANDARDSPAWNTIME;
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

    SDL_Point a, b;
    // SDL_Rect a, b;
    // a.w = a.h = 64;
    // b.w = b.h = 4;
    // Main-loop
    while (true)
    {   
        SDL_Delay(1); 
        TCPdelay = (TCPdelay + 1) % 200;
        UDPreceiveDelay = (UDPreceiveDelay + 1) % 1;
        UDPsendDelay = (UDPsendDelay + 1) % 10;
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
                bool alive = isNetPlayerAlive(server->state, i);
                int health = getNetplayerHealth(server->state, i);
                int lives = getNetplayerLives(server->state, i);
                int kills = getNetPlayerKills(server->state, i);
                bool invulnerable = isNetplayerInvulnerable(server->state, i);

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

                    // Om warmup är igång så ska man direkt kunna lira. Annars måste man vänta tills nästa runda/warmup                   
                    activateNetPlayer(server->state, g);
                    if(server->warmup)
                        {
                            reviveNetPlayer(server->state, g);
                        }
                    
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
                    SDLNet_TCP_AddSocket(server->set, server->tcpsockClient[g]);
                }
            }
        }
        else // Om socketen inte är NULL så betyder det att den är upptagen av någon klient.             
        {
            int ping = 0;
            int response = 0;
          
            SDLNet_CheckSockets(server->set, 0);

            // Kollar om hosten har skickat något kontrollmeddelande och utför motsvarande åtgärd
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

            // Servern "Pingar" alla uppkopplade spelare. Om TCP_Send returnerar en int mindre än "sizeof(ping)" så betyder det att klienten inte svarar och servern gör en plats ledig.
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
    // Loop för kollisionshanteringen för kulor, om spelare dör och om spelet är över
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if(isNetPlayerActive(server->state, i))
        {
            for(int j = 0; j < MAX_BULLETS; j++)
            {
                if(isNetbulletActive(server->state, i, j))
                {                    
                    for (int k = 0; k < MAX_PLAYERS; k++)
                    {
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

                                        // Kollar om endast en spelare är vid liv. Isåfall har den vunnit
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
                                            printf("----------------------------------------\n");
                                            printf("%d seconds | Player % d has won!!!\n", SDL_GetTicks() / 1000, i);
                                            printf("----------------------------------------\n");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } 
        }
    }
    // Loop för respawn
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
    // Loop för odödlighet
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
}

PRIVATE bool circleHitDetect(SDL_Point *a, int rad0, SDL_Point *b, int rad1)
{
    return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y)) < (rad0 + rad1);
}