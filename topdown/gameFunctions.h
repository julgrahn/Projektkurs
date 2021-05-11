#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bullet.h"
#include "button.h"
#include "server.h"
#include "player.h"
#include "sdlinclude.h"
#include "networkgamestate.h"

//void handleMenu(int mouseX, int mouseY, bool shooting, Button buttons[], Bullet bullets[], IPaddress* srvadd, TCPsocket* tcpsock, Networkgamestate networkgamestate, int* playerID,
//   Player players[], UDPsocket* sd, bool* connected, SDL_mutex* mutex, bool* isPlaying, UDPpacket** p2, Server server, bool* host);
void updateplayers(Networkgamestate networkgamestate, Player players[], int playerID);
void playerBulletCollisionCheck(Bullet bullets[], Player players[]);
bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b);
void fire(Bullet bullets[], Player* p, int playerID, int xTarget, int yTarget);

#endif  // GAMEFUNCTIONS_H