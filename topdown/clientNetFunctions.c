#include "clientNetFunctions.h"

#define PUBLIC /* empty */
#define PRIVATE static

typedef struct UDPReceiveStruct_type* UDPReceiveStruct;
struct UDPReceiveStruct_type {
    UDPpacket* p2;
    Player* players;
    Bullet* bullets;
    UDPsocket sd;
    Networkgamestate* state;
    int playerID;
    SDL_mutex* mutex;
};

PRIVATE void UDPReceive(void* args);

PUBLIC void connectToServer(char* ip, IPaddress* srvadd, TCPsocket* tcpsock, Networkgamestate networkgamestate, int* playerID, Player players[], UDPsocket* sd, bool* connected)
{
    printf("Connecting... \n");

    if (SDLNet_ResolveHost(srvadd, ip, 2000) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    *tcpsock = SDLNet_TCP_Open(srvadd);

    if (!*tcpsock)
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    char msg[1024];
    
    if (SDLNet_TCP_Recv(*tcpsock, networkgamestate, getGamestatesize()))
    {
        SDLNet_TCP_Recv(*tcpsock, playerID, sizeof(*playerID));
        //setPlayerAlive(players[*playerID], true);
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            snapPlayer(players[i], getNetworkgamestateplayerX(&networkgamestate, i), getNetworkgamestateplayerY(&networkgamestate, i));
        }

        sprintf(msg, "%d\n", SDLNet_UDP_GetPeerAddress(*sd, -1)->port);
        SDLNet_TCP_Send(*tcpsock, msg, 1024);

        printf("\nConnected with playerID: %d!\n", *playerID);

        *connected = true;
    }
}

PUBLIC void sendUDP(void* player, UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2)
{ 
    memcpy((*p)->data, player, getNetworkplayersize());
    (*p)->address = *srvadd;
    (*p)->len = getNetworkplayersize();
    SDLNet_UDP_Send(*sd, -1, *p);    
}

PUBLIC void startUDPreceiveThread(UDPsocket *sd, UDPpacket** p2, Bullet bullets[][MAX_BULLETS], Player players[], Networkgamestate *networkgamestate, int playerID, SDL_mutex** mutex)
{
    UDPReceiveStruct urs = malloc(sizeof(struct UDPReceiveStruct_type));
    urs->sd = *sd;
    urs->p2 = *p2;
    urs->bullets = *bullets;
    urs->players = players;
    urs->state = networkgamestate;
    urs->playerID = playerID;
    urs->mutex = *mutex;
    SDL_Thread* UDPReceiveThread;
    UDPReceiveThread = SDL_CreateThread((SDL_ThreadFunction)UDPReceive, "UDPReceive", urs);
}

PRIVATE void UDPReceive(void* args)
{
    UDPReceiveStruct urs = (UDPReceiveStruct)args;

    while (true)
    {
        SDL_Delay(1);
        if (SDLNet_UDP_Recv(urs->sd, urs->p2))
        {
            if (SDL_TryLockMutex(urs->mutex) != SDL_MUTEX_TIMEDOUT)
            {
                memcpy(*urs->state, urs->p2->data, getGamestatesize());
                updateplayers(*urs->state, urs->players, urs->playerID);
                updateplayerbullets(*urs->state, urs->playerID, urs->bullets);
                SDL_UnlockMutex(urs->mutex);
            }
        }
    }
}

PUBLIC void handleClientTCP(TCPsocket* tcpsock, SDLNet_SocketSet* set, Networkgamestate networkgamestate, Player players[], int playerID)
{
    SDLNet_CheckSockets(*set, 0);

    if (SDLNet_SocketReady(*tcpsock))
    {
        int response;
        SDLNet_TCP_Recv(*tcpsock, &response, sizeof(response));
        switch (response)
        {
        case 0:
            break;
        case 1:
            SDLNet_TCP_Recv(*tcpsock, networkgamestate, getGamestatesize());
            setPlayerAlive(players[playerID], true);
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                snapPlayer(players[i], getNetworkgamestateplayerX(&networkgamestate, i), getNetworkgamestateplayerY(&networkgamestate, i));
            }
            break;
        default:
            break;
        }
    }
}

void startNewGame(TCPsocket* tcpsock)
{
    int message = 1;
    SDLNet_TCP_Send(*tcpsock, &message, sizeof(message));
}