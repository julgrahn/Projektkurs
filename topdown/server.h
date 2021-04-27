#ifndef SERVER_H_
#define SERVER_H_

#include "sdlinclude.h"

typedef struct Server_type* Server;

Server createServer();
void refreshServer(Server server);


#endif  // SERVER_H_