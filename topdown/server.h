#ifndef SERVER_H_
#define SERVER_H_

#include "sdlinclude.h"

typedef struct Server_type* Server;

Server createServer(Server server);
void refreshServer(Server server);


#endif  // SERVER_H_