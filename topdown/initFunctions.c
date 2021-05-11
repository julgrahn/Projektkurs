#include "initFunctions.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC bool initSDL(SDL_Renderer** renderer)
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
    *renderer = SDL_CreateRenderer(window, 0, renderFlags);
    if (renderer == NULL)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    return true;
}

PUBLIC void initGameObjects(Player players[], Bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i] = createBullet();
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        players[i] = createPlayer(100, 100, i);
    }
    //button = createButton(50, 50);
}

PUBLIC void initClient(UDPsocket* sd, UDPpacket** p, UDPpacket** p2)
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

    if (!((*p = SDLNet_AllocPacket(512)) && (*p2 = SDLNet_AllocPacket(512))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

PUBLIC void loadMenu(SDL_Renderer* renderer, SDL_Texture* connectTextures[], SDL_Texture* hostTextures[], SDL_Texture* quitTextures[])
{
    SDL_Surface* connectButton = IMG_Load("resources/ConnectGrey.png");
    SDL_Surface* connectButtonHover = IMG_Load("resources/ConnectWhite.png");
    SDL_Surface* connectButtonClick = IMG_Load("resources/ConnectGreen.png");

    SDL_Surface* hostButton = IMG_Load("resources/HostGrey.png");
    SDL_Surface* hostButtonHover = IMG_Load("resources/HostWhite.png");
    SDL_Surface* hostButtonClick = IMG_Load("resources/HostGreen.png");

    SDL_Surface* quitButton = IMG_Load("resources/QuitGrey.png");
    SDL_Surface* quitButtonHover = IMG_Load("resources/QuitWhite.png");
    SDL_Surface* quitButtonClick = IMG_Load("resources/QuitRed.png");

    connectTextures[0] = SDL_CreateTextureFromSurface(renderer, connectButton);
    connectTextures[1] = SDL_CreateTextureFromSurface(renderer, connectButtonHover);
    connectTextures[2] = SDL_CreateTextureFromSurface(renderer, connectButtonClick);

    hostTextures[0] = SDL_CreateTextureFromSurface(renderer, hostButton);
    hostTextures[1] = SDL_CreateTextureFromSurface(renderer, hostButtonHover);
    hostTextures[2] = SDL_CreateTextureFromSurface(renderer, hostButtonClick);

    quitTextures[0] = SDL_CreateTextureFromSurface(renderer, quitButton);
    quitTextures[1] = SDL_CreateTextureFromSurface(renderer, quitButtonHover);
    quitTextures[2] = SDL_CreateTextureFromSurface(renderer, quitButtonClick);




    SDL_FreeSurface(connectButton);
    SDL_FreeSurface(connectButtonHover);
    SDL_FreeSurface(connectButtonClick);

    SDL_FreeSurface(hostButton);
    SDL_FreeSurface(hostButtonHover);
    SDL_FreeSurface(hostButtonClick);

    SDL_FreeSurface(quitButton);
    SDL_FreeSurface(quitButtonHover);
    SDL_FreeSurface(quitButtonClick);


    



    /*if (connectTexture == NULL) printf("Finns ingen bild\n");
    else printf("texturen har laddats\n");*/

}

PUBLIC void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], SDL_Texture** pTexture, SDL_Cursor** cursor, SDL_Texture** bulletTexture)
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