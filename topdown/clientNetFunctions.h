#ifndef CLIENTNETFUNCTIONS_H
#define CLIENTNETFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "player.h"
#include "sdlinclude.h"
#include "networkgamestate.h"
#include "gameFunctions.h"

void connectToServer(char* ip, IPaddress* srvadd, TCPsocket* tcpsock, Networkgamestate networkgamestate, int* playerID, Player players[], UDPsocket* sd, bool* connected);
void sendUDP(void* player, UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2);
void startUDPreceiveThread(UDPsocket* sd, UDPpacket** p2, Bullet bullets[], Player players[], Networkgamestate* networkgamestate, int playerID, SDL_mutex** mutex);

#endif  // CLIENTNETFUNCTIONS_H