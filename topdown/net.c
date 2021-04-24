#include "sdlinclude.h"
#include "player.h"

void netInit(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p, UDPpacket *p2);
void netTest(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p, UDPpacket *p2, Player player1, Player player2);

void netInit(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p, UDPpacket *p2)
{
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (!(*sd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Resolve server name  */
    if (SDLNet_ResolveHost(srvadd, "127.0.0.1", 2000) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (!((p = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

}
void netTest(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p, UDPpacket *p2, Player player1, Player player2)
{
    if(player1->oldPosX != player1->pDimensions.x || player1->oldPosY != player1->pDimensions.y)
    {
        sprintf((char *)p->data, "%d %d\n", (int) player1->pDimensions.x, (int) player1->pDimensions.y);
        p->address.host = srvadd->host;
        p->address.port = srvadd->port;
        p->len = strlen((char *)p->data) + 1;
        SDLNet_UDP_Send(*sd, -1, p);
        player1->oldPosX = player1->pDimensions.x;
        player1->oldPosY = player1->pDimensions.y;
    }
    
    if (SDLNet_UDP_Recv(*sd, p2))
    {
        int a, b; 
        sscanf((char * )p2->data, "%d %d\n", &a, &b);
        player2->pDimensions.x = a;
        player2->pDimensions.y = b;
        printf("UDP Packet incoming %d %d\n", a, b);
    }
}