#ifndef SERVER_H_
#define SERVER_H_

#include "sdlinclude.h"

typedef struct Server_type* Server;

Server createServer();
void startServer(Server server);
static void UDPReceive(void* serverPtr);
static void UDPSend(void* serverPtr);


#endif  // SERVER_H_