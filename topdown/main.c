#include <stdio.h>
#include "sdlinclude.h"
#include <stdbool.h>
#include <stdlib.h>
#include "player.h"
#include "world.h"
#include "bullet.h"
#include "server.h"

bool initSDL(SDL_Renderer** renderer);
void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting);
void renderGame(SDL_Renderer* renderer, SDL_Texture* mTiles, SDL_Rect gTiles[], Bullet bullets[], SDL_Texture* bulletTexture, Player players[], SDL_Texture* playerText, SDL_Rect playerRect[], SDL_Point* playerRotationPoint);
void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor, SDL_Texture** bulletTexture);
bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b);
void initClient(UDPsocket *sd, IPaddress *srvadd, UDPpacket **p, UDPpacket **p2, char* ip);
void initGameObjects(Player players[], Bullet bullets[]);
static void TestThread(Server *server);
void startPrompt(int *playerID, Server *server, bool *host);
void fire(Bullet bullets[], Player *p, int *playerID, int xTarget, int yTarget);
void playerBulletCollisionCheck(Bullet bullets[], Player players[]);
void sendReceivePackets(int *sendDelay, int *playerID, int *oldPlayerX, int *oldPlayerY, Player players[], UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2);

int main(int argc, char* args[])
{
    // Variables
    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket* p;
    UDPpacket* p2;
    Server server = NULL;
    int oldPlayerX = 0, oldPlayerY = 0;
    int playerID;
    int sendDelay = 0;
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

    // Init functions
    if (!initSDL(&renderer)) return 1;
    initGameObjects(players, bullets);
    startPrompt(&playerID, &server, &host);
    initClient(&sd, &srvadd, &p, &p2, LOCAL_IP);
    loadMedia(renderer, gridTiles, &tiles, playerRect, &playerText, &cursor, &bulletTexture);

    // Main loop
    while (isPlaying)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouseX, &mouseY, &shooting);

        movePlayer(players[playerID], up, down, right, left, mouseX, mouseY);

        //Flytta på alla andra spelare
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (i != playerID) moveOtherPlayers(players[i]);
        }

        if (shooting) fire(bullets, &players[playerID], &playerID, mouseX, mouseY);

        playerBulletCollisionCheck(bullets, players);
        
        renderGame(renderer, tiles, gridTiles, bullets, bulletTexture, players, playerText, playerRect, &playerRotationPoint);

        sendReceivePackets(&sendDelay, &playerID, &oldPlayerX, &oldPlayerY, players, &sd, &srvadd, &p, &p2);
      
        SDL_Delay(1000 / 60);
        
    }

    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();
    return 0;
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
        SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(players[i])], getPlayerRect(players[i]), getPlayerDirection(players[i]), playerRotationPoint, SDL_FLIP_NONE);
    }
    SDL_RenderPresent(renderer);
}

bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b)
{
    if((a->x)>(b->x) && (a->x)<((b->x)+(b->w)) && (a->y) > (b->y) && (a->y) < ((b->y) + (b->h)))
        return true;
    return false;
}

bool initSDL(SDL_Renderer **renderer)
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
            
          
            case SDL_MOUSEBUTTONDOWN: //KP
           
                *shooting = true;           
                 break;           
            
            case SDL_MOUSEBUTTONUP: //KP
                    *shooting = false;
                    break;

        }
        
    }
    return;
}

void initClient(UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2, char* ip)
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

    printf("Connected to : %s\n", ip);

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
        players[i] = createPlayer(200, 200, i);
    }
}

void startPrompt(int* playerID, Server* server, bool* host)
{
    printf("PlayerID: ");
    scanf(" %d", playerID);
    printf("playerid är :%d\n", *playerID);

    printf("Host(h) or client(c): ");
    char input;
    scanf(" %c", &input);
    if (input == 'h')
    {
        *server = createServer(*server);
        printf("hosted!\n");
        *host = true;
        SDL_Thread* serverThread;
        serverThread = SDL_CreateThread(TestThread, "TestThread", server);
    }
}
static void TestThread(Server *server)
{
    //Uppdatera servern 300ggr / sekunden
    while (true)
    {
        refreshServer(*server);
        SDL_Delay(3);
    }
    
}

void fire(Bullet bullets[], Player *p, int *playerID, int xTarget, int yTarget)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!isBulletActive(bullets[i]))
        {
            spawnBullet(bullets[i], getPlayerX(*p), getPlayerY(*p), xTarget, yTarget, *playerID);
            break;
        }
    }
}

void playerBulletCollisionCheck(Bullet bullets[], Player players[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (isBulletActive(bullets[i]))
        {
            moveBullet(bullets[i]);
            for (int j = 0; j < MAX_PLAYERS; j++)
            {
                if (rectCollisionTest(getBulletRect(bullets[i]), getPlayerRect(players[j]))
                    && (getBulletOwner(bullets[i]) != j))
                {
                    freeBullet(bullets[i]);
                }
            }
            
            
        }
        
    }
}

void sendReceivePackets(int* sendDelay, int* playerID, int* oldPlayerX, int* oldPlayerY, Player players[], UDPsocket* sd, IPaddress* srvadd, UDPpacket** p, UDPpacket** p2)
{
     // Send
     if (*sendDelay >= 5) // Skickar paket högst var femte frame
        {           
            if (getPlayerX(players[*playerID]) != *oldPlayerX || getPlayerY(players[*playerID]) != *oldPlayerY)
            {
                
                sprintf((char*)(*p)->data, "%d %d %d\n", getPlayerX(players[*playerID]), getPlayerY(players[*playerID]), getPlayerID(players[*playerID]));
                (*p)->address.host = srvadd->host;
                (*p)->address.port = srvadd->port;
                (*p)->len = strlen((char*)(*p)->data) + 1;
                SDLNet_UDP_Send(*sd, -1, *p);
                *oldPlayerX = getPlayerX(players[*playerID]);
                *oldPlayerY = getPlayerY(players[*playerID]);
            }
            *sendDelay = 0;
        }
        else
        {
            (*sendDelay)++;
        }    

     // Receive
     if (SDLNet_UDP_Recv(*sd, *p2)) {
         int a, b, c;
         sscanf((char*)(*p2)->data, "%d %d %d\n", &a, &b, &c);
         updatePlayerPosition(players[c], a, b);
     }
    
}