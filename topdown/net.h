#ifndef NET_H
#define NET_H

#include "sdlinclude.h"
#include "player.h"

void netInit(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p, UDPpacket *p2);
void netTest(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p, UDPpacket *p2, Player player1, Player player2);

#endif