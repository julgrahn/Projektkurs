#ifndef SERVER_H_
#define SERVER_H_

#include "sdlinclude.h"
#include "player.h"
#include "bullet.h"

typedef struct Server_type* Server;

Server createServer();
void startServer(Server server);
// void playerBulletCollisionCheck(Bullet bullets[], Player players[]);
void fire(Bullet bullets[], Player* p, int playerID, int xTarget, int yTarget);
bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b);


#endif  // SERVER_H_