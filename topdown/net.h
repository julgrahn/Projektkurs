#ifndef NET_H
#define NET_H

#include "sdlinclude.h"
#include "player.h"

void netInit(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p);
void netTest(IPaddress *srvadd, UDPsocket *sd, UDPpacket *p);

// void netUpdate(int &x_posOld, int &y_posOld, int &x_pos, int &y_pos);

#endif