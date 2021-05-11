#include "initFunctions.h"

#define PUBLIC /* empty */
#define PRIVATE static

PUBLIC bool initSDL(SDL_Renderer** renderer, Mix_Chunk** sound)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }
    Mix_AllocateChannels(100);
    return true;
}

PUBLIC void initGameObjects(Player players[], Bullet bullets[][MAX_BULLETS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            bullets[i][j] = createBullet();
        }
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        players[i] = createPlayer(100, 100, i);
    }
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

    if (!((*p = SDLNet_AllocPacket(1024)) && (*p2 = SDLNet_AllocPacket(1024))))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

PUBLIC void loadMedia(SDL_Renderer* renderer, SDL_Rect gTiles[], SDL_Texture** tiles, SDL_Rect playerRect[], 
                        SDL_Texture** pTexture, SDL_Cursor** cursor, SDL_Texture** bulletTexture, 
                        SDL_Texture** gunFireTexture, SDL_Texture** explosionTexture, 
                        SDL_Texture** bloodTexture, Mix_Chunk** sound,
                        SDL_Rect explosionTiles[], SDL_Rect bloodTiles[])
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

    SDL_Surface* bulletSurface = IMG_Load("resources/expl_04_0014.png");
    *bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);

    SDL_Surface* gunFireSurface = IMG_Load("resources/muzzle2_0007.png");
    *gunFireTexture = SDL_CreateTextureFromSurface(renderer, gunFireSurface);
    SDL_FreeSurface(gunFireSurface);

    SDL_Surface* explosionSurface = IMG_Load("resources/explosions.png");
    *explosionTexture = SDL_CreateTextureFromSurface(renderer, explosionSurface);
    SDL_FreeSurface(explosionSurface);
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            explosionTiles[i * 11 + j].x = j * 93;
            explosionTiles[i * 11 + j].y = i * 93;
            explosionTiles[i * 11 + j].w = 93;
            explosionTiles[i * 11 + j].h = 93;
        }
    }

    // Blod
    SDL_Surface* bloodSurface = IMG_Load("resources/blood - right 1.png");
    *bloodTexture = SDL_CreateTextureFromSurface(renderer, bloodSurface);
    SDL_FreeSurface(bloodSurface);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            bloodTiles[i * 4 + j].x = j * 512;
            bloodTiles[i * 4 + j].y = i * 512;
            bloodTiles[i * 4 + j].w = 512;
            bloodTiles[i * 4 + j].h = 512;
        }
    }


    // Gunfire Soundeffect
    *sound = Mix_LoadWAV("resources/gunsound2.wav");
    if (sound == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Volume
    Mix_Volume(-1, 20);



}