#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static
#define SERVER_REFRESH_RATE 10  // 30 = 10/s    10 = 30/s

struct Server_type {
    UDPsocket sd;       /* Socket descriptor */
    UDPpacket* pRecive;       /* Pointer to packet memory */
    UDPpacket* pSent;
    TCPsocket tcpsockServer;
    TCPsocket tcpsockClient[MAX_PLAYERS + 1];
    IPaddress serverIP;
    IPaddress clients[MAX_PLAYERS];
    int noOfPlayers;
    int xPos[MAX_PLAYERS], yPos[MAX_PLAYERS], pID[MAX_PLAYERS];
    double pDirection[MAX_PLAYERS];
    int playerX[MAX_PLAYERS]; // Spelar-X
    int playerY[MAX_PLAYERS]; // Spelar-Y
    int playerID[MAX_PLAYERS]; // SpelarID
    int playerRotation[MAX_PLAYERS]; // Player rotation
    int playerShooting[MAX_PLAYERS];
    int pMouseX[MAX_PLAYERS];
    int pMouseY[MAX_PLAYERS];
    SDL_Thread* UDPreceiveThread;
    SDL_Thread* UDPsendThread;
};

PUBLIC Server createServer()
{

    Server server = malloc(sizeof(struct Server_type));

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
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        server->playerX[i] = 200; // Spelar-X
        server->playerY[i] = 200; // Spelar-Y
        server->playerID[i] = -1; // SpelarID, 0 till 4. -1 innebär att spelaren inte är ansluten
        server->playerRotation[i] = 0; // Player rotation
        server->playerShooting[i] = 0;
        server->pMouseX[i] = 0;
        server->pMouseY[i] = 0;
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
                                if (server->playerID[i] == -1)
                                {
                                    newPlayerID = i;
                                    server->playerID[i] = i;
                                    break;
                                }
                            }
                            // Tilldela spelarID och skicka startpositionerna för alla spelare
                            sprintf(msg, "%d %d %d %d %d %d %d %d %d %d %d\n", newPlayerID, server->playerX[0], server->playerY[0], server->playerX[1], server->playerY[1], server->playerX[2], server->playerY[2], server->playerX[3], server->playerY[3], server->playerX[4], server->playerY[4]);
                            len = strlen(msg) + 1;
                            SDLNet_TCP_Send(server->tcpsockClient[g], msg, len);
                            server->clients[g].host = SDLNet_TCP_GetPeerAddress(server->tcpsockClient[g])->host;

                            // Klienten behöver skicka sin lokala UDP-port till servern så servern vet vilken port den ska skicka paketen till.
                            SDLNet_TCP_Recv(server->tcpsockClient[g], msg, 1024);
                            sscanf((char*)msg, "%d\n", &(server->clients[g].port));
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
                        server->playerID[g] = -1;                       
                        server->clients[g].host = NULL;
                        server->clients[g].port = NULL;
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
                for (int i = 0; i < server->noOfPlayers; i++)
                {
                    if (server->pRecive->address.port == server->clients[i].port) // Kollar vem paket kom ifrån och uppdaterar endast den spelaren
                    {
                        sscanf((char*)server->pRecive->data, "%d %d %d %d %d %d %d\n",
                            &server->playerX[i], &server->playerY[i],
                            &server->playerID[i], &server->playerRotation[i],
                            &server->playerShooting[i], &server->pMouseX[i],
                            &server->pMouseY[i]);
                    }
                }
            }
        }
        
       

        // UDP Send
        if (!UDPsendDelay)
        {
            // Spelar i's information skickas till spelare j
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                for (int j = 0; j < MAX_PLAYERS; j++)
                {
                    if (i != j && (server->tcpsockClient[i] != NULL) && (server->tcpsockClient[j] != NULL)) // Paket skickas bara till de som är inne
                    {
                        server->pSent->address = server->clients[j];	// Set the destination host 
                        sprintf((char*)server->pSent->data, "%d %d %d %d %d %d %d\n",
                            server->playerX[i], server->playerY[i],
                            server->playerID[i], (int)server->playerRotation[i],
                            server->playerShooting[i], server->pMouseX[i], server->pMouseY[i]);

                        server->pSent->len = strlen((char*)server->pSent->data) + 1;
                        SDLNet_UDP_Send(server->sd, -1, server->pSent);
                    }
                }
            }
        }
    }         
}


// Trådar som inte används just nu

PUBLIC static void UDPReceive(void* serverPtr)
{
    Server server = (Server)serverPtr;
    while (true)
    {      
        SDL_Delay(1); // Hur ofta servern scannar för att ta emot paket
        if (SDLNet_UDP_Recv(server->sd, server->pRecive))
        {          
            for (int i = 0; i < server->noOfPlayers; i++)
            {
                if (server->pRecive->address.port == server->clients[i].port) // KOllar vem paket kom ifrån och uppdaterar endast den spelaren
                {
                    sscanf((char*)server->pRecive->data, "%d %d %d %d\n", &server->playerX[i], &server->playerY[i], &server->playerID[i], &server->playerRotation[i]);
                }
            }          
        }        
    }     
}

PUBLIC static void UDPSend(void* serverPtr)
{
    Server server = (Server)serverPtr;
    while (true)
    {
        SDL_Delay(100); // Hur ofta servern skickar paket

        // Spelar i's information skickas till spelare j
        for (int i = 0; i < server->noOfPlayers; i++)
        {
            for (int j = 0; j < server->noOfPlayers; j++)
            {
                if (i != j)
                {

                    server->pSent->address = server->clients[j];	// Set the destination host 
                    sprintf((char*)server->pSent->data, "%d %d %d %d\n", server->playerX[i], server->playerY[i], server->playerID[i], (int)server->playerRotation[i]);
                    server->pSent->len = strlen((char*)server->pSent->data) + 1;
                    SDLNet_UDP_Send(server->sd, -1, server->pSent);                 
                }
            }
        }
    }
    
}