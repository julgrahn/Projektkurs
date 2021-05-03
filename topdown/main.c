#pragma warning(disable : 4996)

#include <stdio.h>
#include "sdlinclude.h"
#include <stdbool.h>
#include <stdlib.h>
#include "player.h"
#include "world.h"
#include "bullet.h"
#include "server.h"
#include "networkgamestate.h"

#define TICKRATE 2 // Number of frames per network-packet

typedef struct UDPReceiveStruct_type* UDPReceiveStruct;
struct UDPReceiveStruct_type {
    UDPpacket* p2;
    Player* players;
    Bullet* bullets;
    UDPsocket sd;
    Networkgamestate* state;
};

bool initSDL(SDL_Renderer** renderer);
void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting);
void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[], SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint);
void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor, SDL_Texture** bulletTexture);
bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b);
void initClient(UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2, char* ip, TCPsocket* tcpsock, int* localPort);
void initGameObjects(Player players[], Bullet bullets[]);
void sendReceivePackets(int sendDelay, Networkgamestate* state, Networkplayer player, UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2, Player players[]);
void updateplayers(Networkgamestate networkgamestate, Player players[]);
static void TestThread(void* server);
void startPrompt(int* playerID, Server* server, bool* host);
void fire(Bullet bullets[], Player* p, int playerID, int xTarget, int yTarget);
void playerBulletCollisionCheck(Bullet bullets[], Player players[], int* playerID);
static void UDPReceive(void* args);

int main(int argc, char* args[])
{
    // Variables
    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    UDPsocket sd;
    TCPsocket tcpsock = NULL;
    IPaddress srvadd;
    UDPpacket* p;
    UDPpacket* p2;
    int localPort;
    Server server = NULL;
    int oldPlayerX = 0, oldPlayerY = 0;
    int playerID;
    SDL_Cursor* cursor = NULL;
    Player players[MAX_PLAYERS];
    SDL_Texture* playerText;
    SDL_Rect playerRect[4];
    int mouseX = 0, mouseY = 0;
    Bullet bullets[MAX_BULLETS];
    SDL_Texture* tiles = NULL;
    SDL_Rect gridTiles[900];   // Kommer innehålla alla 900 rutor från bakgrundsbilden, kan optmiseras.
    bool isPlaying = true, shooting = false, host = false;
    SDL_Texture* bulletTexture = NULL;
    int up = 0, down = 0, left = 0, right = 0;
    SDL_Point playerRotationPoint = { 20, 32 };

    Networkgamestate networkgamestate = createNetworkgamestate();
    Networkplayer playertest;
    Uint32 fpsTimerStart, frameTicks;
    // Init functions
    if (!initSDL(&renderer)) return 1;
    initGameObjects(players, bullets);
    startPrompt(&playerID, &server, &host);
    initClient(&sd, &srvadd, &p, &p2, ALEX_IP, &tcpsock, &localPort);
    loadMedia(renderer, gridTiles, &tiles, playerRect, &playerText, &cursor, &bulletTexture);

    // TCP för programstart. Man kan inte lämna loopen förrän man har anslutit till servern
    bool connected = false;
    char msg[1024];
    while (!connected)
    {
        printf("Connecting... \n");
        if (SDLNet_TCP_Recv(tcpsock, &networkgamestate, sizeof(networkgamestate)))
        {
            SDLNet_TCP_Recv(tcpsock, &playerID, sizeof(playerID));
            if (playerID == -1)
            {
                return 1; // Stäng av programmet om servern är full
            }
            playertest = createNetworkplayer(playerID);
            playertest.isAlive = true;
            setPlayerAlive(players[playerID], true);
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                snapPlayer(players[i], networkgamestate.players[i].posX, networkgamestate.players[i].posY);
            }

            sprintf(msg, "%d\n", SDLNet_UDP_GetPeerAddress(sd, -1)->port);
            SDLNet_TCP_Send(tcpsock, msg, 1024);

            printf("\nConnected with playerID: %d!\n", playerID);

            connected = true;
        }
    }

    UDPReceiveStruct urs = malloc(sizeof(struct UDPReceiveStruct_type));
    urs->sd = sd;
    urs->p2 = p2;
    urs->bullets = bullets;
    urs->players = players;
    urs->state = &networkgamestate;

    SDL_Thread* UDPReceiveThread;
    UDPReceiveThread = SDL_CreateThread(UDPReceive, "UDPReceive", urs);

    // Main loop
    while (isPlaying)
    {
        fpsTimerStart = SDL_GetTicks();
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouseX, &mouseY, &shooting);
        setPlayerShooting(&players[playerID], shooting, mouseX, mouseY);
        if (isPlayerAlive(players[playerID]))
        {
            movePlayer(players[playerID], up, down, right, left, mouseX, mouseY);
            if (isPlayershooting(players[playerID])) fire(bullets, &players[playerID], playerID, mouseX, mouseY);
        }
        //Flytta på alla andra spelare
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (i != playerID) {
                moveOtherPlayers(players[i]);
                if (isPlayershooting(players[i]))
                    fire(bullets, &players[i], i, getPlayerxtarget(players[i]), getPlayerytarget(players[i]));
            }
        }
        playerBulletCollisionCheck(bullets, players, &playerID);
        setNetworkplayer(&playertest, players[playerID]);
        sendReceivePackets(TICKRATE, &networkgamestate, playertest, &sd, &srvadd, &p, &p2, players);
        renderGame(renderer, tiles, gridTiles, bullets, bulletTexture, players, playerText, playerRect, &playerRotationPoint);
        frameTicks = SDL_GetTicks() - fpsTimerStart;
        if (frameTicks < (1000 / 60))
        {
            SDL_Delay((1000 / 60) - frameTicks);
        }
        // printf("%u\n", frameTicks);
    }

    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();
    return 0;
}

void updateplayers(Networkgamestate networkgamestate, Player players[])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        updatePlayerPosition(&players[i], networkgamestate.players[i].posX, networkgamestate.players[i].posY, networkgamestate.players[i].direction, networkgamestate.players[i].isAlive, networkgamestate.players[i].isShooting, networkgamestate.players[i].xTarget, networkgamestate.players[i].yTarget);
    }
}

void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor, SDL_Texture** bulletTexture)
{
    SDL_Surface* gTilesSurface = IMG_Load("resources/tilemap.png");
    *tiles = SDL_CreateTextureFromSurface(renderer, gTilesSurface);
    SDL_FreeSurface(gTilesSurface);
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            gTiles[i * 30 + j].x = j * getTileWidth();
            gTiles[i * 30 + j].y = i * getTileHeight();
            gTiles[i * 30 + j].w = getTileWidth();
            gTiles[i * 30 + j].h = getTileHeight();
        }

    }

    SDL_Surface* playerSurface = IMG_Load("resources/playerRifle.png");
    *pTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    for (int n = 0; n < 4; n++)
    {
        playerRect[n].x = 0 + (n * 64);
        playerRect[n].y = 0;
        playerRect[n].h = 64;
        playerRect[n].w = 64;
    }

    SDL_Surface* cursorSurface = IMG_Load("resources/crosshair161.png");
    *cursor = SDL_CreateColorCursor(cursorSurface, 36, 36);
    SDL_FreeSurface(cursorSurface);
    SDL_SetCursor(*cursor);

    SDL_Surface* bulletSurface = IMG_Load("resources/bullet.png");
    *bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
}

void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[],
    SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint)
{
    SDL_RenderClear(renderer);
    // Render Background
    SDL_Rect position;
    position.y = 0;
    position.x = 0;
    position.h = getTileHeight();
    position.w = getTileWidth();

    for (int i = 0; i < getTileColumns(); i++)
    {
        for (int j = 0; j < getTileRows(); j++)
        {
            position.y = i * getTileHeight();
            position.x = j * getTileWidth();
            SDL_RenderCopyEx(renderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
    // Render Bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (isBulletActive(bullets[i]))
        {
            SDL_RenderCopy(renderer, bulletTexture, NULL, getBulletRect(bullets[i]));
        }
    }
    // Render Players
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (isPlayerAlive(players[i]))
        {
            SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(players[i])], getPlayerRect(players[i]), getPlayerDirection(players[i]), playerRotationPoint, SDL_FLIP_NONE);
        }
    }
    SDL_RenderPresent(renderer);
}

bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b)
{
    if ((a->x) > (b->x) && (a->x) < ((b->x) + (b->w)) && (a->y) > (b->y) && (a->y) < ((b->y) + (b->h)))
        return true;
    return false;
}

bool initSDL(SDL_Renderer** renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    SDL_Window* window = SDL_CreateWindow("top down extreme shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWWIDTH, WINDOWHEIGHT, 0);
    if (window == NULL)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    *renderer = SDL_CreateRenderer(window, -1, renderFlags);
    if (renderer == NULL)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    return true;
}

void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting)
{
    SDL_GetMouseState(mouseX, mouseY);
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
        case SDL_QUIT:
            *isPlaying = false;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                *up = 1;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                *left = 1;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                *down = 1;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                *right = 1;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event->key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                *up = 0;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                *left = 0;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                *down = 0;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                *right = 0;
                break;
            default:
                break;
            }
            break;


        case SDL_MOUSEBUTTONDOWN:

            switch (event->button.button)
            {
            case SDL_BUTTON_LEFT:
                *shooting = true;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:

            switch (event->button.button)
            {
            case SDL_BUTTON_LEFT:
                *shooting = false;
            default:
                break;
            }
            break;

        }

    }
    return;
}

void initClient(UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2, char* ip, TCPsocket* tcpsock, int* localPort)
{
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if (!(*sd = SDLNet_UDP_Open(0)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    // Resolve server name  
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
    if (!((*p = SDLNet_AllocPacket(512)) && (*p2 = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }


}

void initGameObjects(Player players[], Bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i] = createBullet();
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        players[i] = createPlayer(100, 100, i);
    }
}

void startPrompt(int* playerID, Server* server, bool* host)
{
    

    bool check = false; 

    while (!check)
    {
        *server = createServer();
        printf("hosted!\n");
        *host = true;
        SDL_Thread* serverThread;
        serverThread = SDL_CreateThread(TestThread, "TestThread", server);
    }
    
}

static void TestThread(void* server)
{
    startServer(*(Server*)server);
}

void fire(Bullet bullets[], Player* p, int playerID, int xTarget, int yTarget)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!isBulletActive(bullets[i]))
        {
            spawnBullet(bullets[i], getPlayerX(*p), getPlayerY(*p), xTarget, yTarget, playerID);
            break;
        }
    }
}

void playerBulletCollisionCheck(Bullet bullets[], Player players[], int* playerID)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (isBulletActive(bullets[i]))
        {
            moveBullet(bullets[i]);
            for (int j = 0; j < MAX_PLAYERS; j++)
            {
                if (rectCollisionTest(getBulletRect(bullets[i]), getPlayerRect(players[j]))
                    && (getBulletOwner(bullets[i]) != j) && isPlayerAlive(players[j]))
                {
                    freeBullet(bullets[i]);
                    if (j == *playerID)
                    {
                        damagePlayer(players[j], getBulletDamage(bullets[j]));
                    }
                }
            }
        }
    }
}

void sendReceivePackets(int sendDelay, Networkgamestate* state, Networkplayer player, UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2, Player players[])
{
    // Send
    static int send = 0;
    if (sendDelay) send = (send + 1) % sendDelay;
    if (1) // Skickar paket 60/sek
    {
        memcpy((*p)->data, &player, sizeof(player));
        (*p)->address = *srvadd;
        (*p)->len = sizeof(player);
        SDLNet_UDP_Send(*sd, -1, *p);
    }
}

static void UDPReceive(void* args)
{
    UDPReceiveStruct urs = (UDPReceiveStruct)args;

    while (true)
    {
        SDL_Delay(1);
        if (SDLNet_UDP_Recv(urs->sd, urs->p2))
        {
            memcpy(urs->state, urs->p2->data, sizeof(*urs->state));
            updateplayers(*urs->state, urs->players);
        }
    }
}