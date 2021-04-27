#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static
#define SERVER_REFRESH_RATE 30  // 30 = 10/s    10 = 30/s

struct Server_type {
    UDPsocket sd;       /* Socket descriptor */
    UDPpacket* pRecive;       /* Pointer to packet memory */
    UDPpacket* pSent;
    TCPsocket tcpsockServer;
    TCPsocket tcpsockClient;
    IPaddress serverIP;
    Uint32 IPclients[MAX_PLAYERS];
    Uint32 portClients[MAX_PLAYERS];
    int noOfPlayers;
    int xPos[MAX_PLAYERS], yPos[MAX_PLAYERS], pID[MAX_PLAYERS];
    double pDirection[MAX_PLAYERS];
    int send;
    int a[MAX_PLAYERS]; // Spelar-X
    int b[MAX_PLAYERS]; // Spelar-Y
    int c[MAX_PLAYERS]; // SpelarID
    int d[MAX_PLAYERS]; // Player rotation
};

PUBLIC Server createServer()
{

    Server server = malloc(sizeof(struct Server_type));
    server->send = 0;
    server->tcpsockClient = NULL;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        server->IPclients[i] = 0;
        server->portClients[i] = 0;
    }
    server->noOfPlayers = 0;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        server->a[i] = 200; // Spelar-X
        server->b[i] = 200; // Spelar-Y
        server->c[i] = 0; // SpelarID
        server->d[i] = 0; // Player rotation
    }

    //Initialize SDL_net 
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (SDLNet_ResolveHost(&server->serverIP, NULL, 2000) < 0) 
    { 
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (!(server->tcpsockServer = SDLNet_TCP_Open(&server->serverIP)))
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

PUBLIC void refreshServer(Server server)
{
    Uint32 data;
    server->send = (server->send + 1) % SERVER_REFRESH_RATE;
    /* Wait a packet. UDP_Recv returns != 0 if a packet is coming */

    // TCP
    if (server->tcpsockClient == NULL)
    {
        server->tcpsockClient = SDLNet_TCP_Accept(server->tcpsockServer);
        if (server->tcpsockClient)
        {
            char msg[1024];
            int len;

            // Tilldela spelarID och skicka startpositionerna för alla spelare
            sprintf(msg, "%d %d %d %d %d %d %d %d %d %d %d", server->noOfPlayers, server->a[0], server->b[0], server->a[1], server->b[1], server->a[2], server->b[2], server->a[3], server->b[3], server->a[4], server->b[4]);
            len = strlen(msg) + 1;
            SDLNet_TCP_Send(server->tcpsockClient, msg, len);    
            SDLNet_TCP_Close(server->tcpsockClient);
            server->tcpsockClient = NULL;
        }
    }
    
    // UDP
    if (SDLNet_UDP_Recv(server->sd, server->pRecive))
    {
        // Om en ny klient ansluter
        for (int i = server->noOfPlayers; i < MAX_PLAYERS; i++)
        {
            if (server->IPclients[i] == 0 && server->portClients[i] == 0
                && server->pRecive->address.port != server->portClients[0]
                && server->pRecive->address.port != server->portClients[1]
                && server->pRecive->address.port != server->portClients[2]
                && server->pRecive->address.port != server->portClients[3]
                && server->pRecive->address.port != server->portClients[4])
            {
                server->noOfPlayers++;
                server->IPclients[i] = server->pRecive->address.host;
                server->portClients[i] = server->pRecive->address.port;
                break;
            }
        }

        // Hanterandet av paket
        // i �r den som skickar
        // j �r den som tar emot

        for (int i = 0; i < server->noOfPlayers; i++)
        {
            // if (server->pRecive->address.port == server->portClients[i])
            if (server->pRecive->address.port == server->portClients[i])
            {
                sscanf((char*)server->pRecive->data, "%d %d %d %d\n", &server->a[i], &server->b[i], &server->c[i], &server->d[i]);
            }
        }
       
    }
    if (!server->send)
    {
        for (int i = 0; i < server->noOfPlayers; i++)
        {
            for (int j = 0; j < server->noOfPlayers; j++)
            {
                if (i != j)
                {
                    server->pSent->address.host = server->IPclients[j];	// Set the destination host 
                    server->pSent->address.port = server->portClients[j];
                    sprintf((char*)server->pSent->data, "%d %d %d %d\n", server->a[i], server->b[i], server->c[i], (int)server->d[i]);
                    server->pSent->len = strlen((char*)server->pSent->data) + 1;
                    SDLNet_UDP_Send(server->sd, -1, server->pSent);
                    // SDL_Delay(10);
                }
            }
        }
    }
}