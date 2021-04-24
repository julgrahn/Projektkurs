#include <stdio.h>
#include "sdlinclude.h"
#include <stdbool.h>
#include <stdlib.h>
#include "player.h"
#include "world.h"
#include "bullet.h"
#include "server.h"


bool init(SDL_Renderer** renderer);
void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting);
void renderBackground(SDL_Renderer* gRenderer, SDL_Texture* mTiles, SDL_Rect gTiles[]);
void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor);
bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b);
void initClient(UDPsocket *sd, IPaddress *srvadd, UDPpacket **p, UDPpacket **p2, char* ip);


int main(int argc, char* args[])
{
    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    if (!init(&renderer)) return 1;

    SDL_Cursor* cursor = NULL;

    // Player
    Player players[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        players[i] = createPlayer(200, 200, i);
    }
    //Player player1 = createPlayer(200, 200);
    SDL_Texture* playerText;
    SDL_Rect playerRect[4];
    int mouseX = 0, mouseY = 0;

    //Player dummy = createPlayer(100, 100);
    

    // Bullet
    Bullet bullets[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i] = createBullet();
    }
    SDL_Surface* bulletSurface = IMG_Load("resources/bullet.png");
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);

    bool isPlaying = true, shooting = false;
    int up = 0, down = 0, left = 0, right = 0;

    // Background
    SDL_Texture* tiles = NULL;
    SDL_Rect gridTiles[900];   // Kommer innehålla alla 900 rutor från bakgrundsbilden, kan optmiseras.

    // Netinit
    UDPsocket sd;
    IPaddress srvadd;
    UDPpacket* p;
    UDPpacket* p2;
    Server server = NULL;
    bool host = false;
    int oldPlayerX = 0, oldPlayerY = 0;
    int playerID;
    printf("PlayerID: ");
    
    scanf(" %d", &playerID);
    printf("palyerid är :%d\n", playerID);
    printf("Host(h) or client(c): ");
    char input;
    scanf(" %c", &input);
    if (strcmp(&input, "h") == 1)
    {
        server = createServer(server);
        printf("hosted!\n");
        host = true;
    }
    initClient(&sd, &srvadd, &p, &p2, ANDREAS_IP);
    

    loadMedia(renderer, gridTiles, &tiles, playerRect, &playerText, &cursor);

    SDL_Point playerRotationPoint = { 20, 32 };

    while (isPlaying)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouseX, &mouseY, &shooting);

        movePlayer(players[playerID], up, down, right, left, mouseX, mouseY);
        if (shooting)
        {
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!isBulletActive(bullets[i]))
                {
                    spawnBullet(bullets[i], getPlayerX(players[playerID]), getPlayerY(players[playerID]), getPlayerDirection(players[playerID]));
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (isBulletActive(bullets[i]))
            {
                moveBullet(bullets[i]);
                //if (rectCollisionTest(getBulletRect(bullets[i]), getPlayerRect(dummy)))
                //{
                //    freeBullet(bullets[i]);
                //}
            }
        }

        
        
        SDL_RenderClear(renderer);

        //Game renderer
        renderBackground(renderer, tiles, gridTiles);

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            SDL_RenderCopyEx(renderer, playerText, &playerRect[getPlayerFrame(players[i])], getPlayerRect(players[i]), getPlayerDirection(players[i]), &playerRotationPoint, SDL_FLIP_NONE);
        }
        
       
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (isBulletActive(bullets[i]))
            {
                SDL_RenderCopy(renderer, bulletTexture, NULL, getBulletRect(bullets[i]));
            }
            
        }

        if (getPlayerX(players[playerID]) != oldPlayerX || getPlayerY(players[playerID]) != oldPlayerY)
        {
            sprintf((char*)p->data, "%d %d %d\n", getPlayerX(players[playerID]), getPlayerY(players[playerID]), getPlayerID(players[playerID]));
            p->address.host = srvadd.host;
            p->address.port = srvadd.port;
            p->len = strlen((char*)p->data) + 1;
            SDLNet_UDP_Send(sd, -1, p);
            oldPlayerX = getPlayerX(players[playerID]);
            oldPlayerY = getPlayerY(players[playerID]);
        }
        

         if (SDLNet_UDP_Recv(sd, p2)){
            int a, b, c; 
            sscanf((char * )p2->data, "%d %d %d\n", &a, &b, &c);
            updatePlayerPosition(players[c], a, b);

            // printf("UDP Packet incoming %d %d %d\n", a, b, c);
        }
   
        if(host) refreshServer(server);
        
        
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
        
    }


    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();

    return 0;
}

void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor)
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
}

void renderBackground(SDL_Renderer* gRenderer, SDL_Texture* mTiles, SDL_Rect gTiles[])
{
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
            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
}

bool rectCollisionTest(SDL_Rect* a, SDL_Rect* b)
{
    if((a->x)>(b->x) && (a->x)<((b->x)+(b->w)) && (a->y) > (b->y) && (a->y) < ((b->y) + (b->h)))
        return true;
    return false;
}

bool init(SDL_Renderer **renderer)
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

    if (!((*p = SDLNet_AllocPacket(32)) && (*p2 = SDLNet_AllocPacket(32))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}