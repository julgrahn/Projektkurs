#ifndef SERVER_H_
#define SERVER_H_

#include "sdlinclude.h"
#include "player.h"
#include "bullet.h"
#include "gameFunctions.h"

typedef struct Server_type* Server;

Server createServer();
void startServer(Server server);

#endif  // SERVER_H_