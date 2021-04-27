#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

struct Server_type {
    UDPsocket sd;       /* Socket descriptor */
    UDPpacket* pRecive;       /* Pointer to packet memory */
    UDPpacket* pSent;
    Uint32 IPclients[MAX_PLAYERS];
    Uint32 portClients[MAX_PLAYERS];
    int noOfPlayers;
};

PUBLIC Server createServer()
{
    Server server = malloc(sizeof(struct Server_type));

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        server->IPclients[i] = 0;
        server->portClients[i] = 0;
    }

    server->noOfPlayers = 0;

    //Initialize SDL_net 
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
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
    int a = 0; // Spelar-X
    int b = 0; // Spelar-Y
    int c = 0; // SpelarID
    double d = 0; // Player rotation

    /* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
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
        for (int i = 0; i < server->noOfPlayers+1; i++)
        {
            if (server->pRecive->address.port == server->portClients[i])
            {
                for (int j = 0; j < server->noOfPlayers; j++)
                {
                    if (i != j)
                    {
                        server->pSent->address.host = server->IPclients[j];	// Set the destination host 
                        server->pSent->address.port = server->portClients[j];
                        sscanf((char*)server->pRecive->data, "%d %d %d %lf\n", &a, &b, &c, &d);
                        sprintf((char*)server->pSent->data, "%d %d %d %lf\n", a, b, c, d);
                        server->pSent->len = strlen((char*)server->pSent->data) + 1;
                        SDLNet_UDP_Send(server->sd, -1, server->pSent);
                        //printf("Client %d skickar till Client %d\n", i+1, j+1);
                    }
                }
            }
        }
    }
}
