#include "sdlinclude.h"
#include "player.h"

void netInit(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p);
void netTest(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p);

void netInit(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p)
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
void netTest(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p){

    sprintf((char *)p->data, "%d %d\n", 100, 200);
    p->address.host = (*srvadd).host;	/* Set the destination host */
    p->address.port = (*srvadd).port;	/* And destination port */
    p->len = strlen((char *)p->data) + 1;
    SDLNet_UDP_Send(*sd, -1, p);
}