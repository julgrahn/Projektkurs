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
    SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
                        SDL_Texture** gunFireTexture, SDL_Rect gunFireRect, SDL_Texture** explosionTexture, 
                        SDL_Rect explosionRect, SDL_Texture** gunFireTexture2, SDL_Rect gunFireRect2, Mix_Chunk** sound)
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
     

    SDL_Surface* gunFireSurface = IMG_Load("resources/muzzle2_0007.png");
    *gunFireTexture = SDL_CreateTextureFromSurface(renderer, gunFireSurface);
    SDL_FreeSurface(gunFireSurface);
    // gunFireRect.x = 0;
    // gunFireRect.y = 0;
    // gunFireRect.h = 16;
    // gunFireRect.w = 16;

    SDL_Surface* explosionSurface = IMG_Load("resources/expl_01_0002.png");
    *explosionTexture = SDL_CreateTextureFromSurface(renderer, explosionSurface);
    SDL_FreeSurface(explosionSurface);

    SDL_Surface* gunFireSurface2 = IMG_Load("resources/muzzle2.2.png");
    *gunFireTexture2 = SDL_CreateTextureFromSurface(renderer, gunFireSurface2);
    SDL_FreeSurface(gunFireSurface2);


    // Gunfire Soundeffect
    *sound = Mix_LoadWAV("resources/gunsound2.wav");
    if (sound == NULL)
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    // Volume
    Mix_Volume(-1, 20);
}

PUBLIC void initGameHUD(SDL_Renderer *renderer, SDL_Rect textRect[], SDL_Texture **textTexture, SDL_Rect *healthBar, SDL_Rect *reloadTimer)
{
    TTF_Init();
    TTF_Font *font;
    font=TTF_OpenFont("unispace_bd.ttf", 20);
    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    else
    {
        SDL_Color color = {255,255,255};
        SDL_Surface *textSurface;
        textSurface = TTF_RenderText_Solid(font,"0123456789|HP: ",color);
        *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        healthBar->h = 10, healthBar->w = 150, healthBar->x = 100, healthBar->y = 7;
        reloadTimer->x = 35, reloadTimer->y = 2, reloadTimer->w = 24; 
        int width, height;
        TTF_SizeText(font, "0123456789|HP: ", &width, &height);
        for(int i = 0; i < 11; i++)
        {
            textRect[i].x = i * width/15;
            textRect[i].y = 0;
            textRect[i].w = width/15;
            textRect[i].h = height;
        }
        for(int i = 12; i < 15; i++)
        {
            textRect[i].x = i * width/15 + (width/15*4);
            textRect[i].y = 0;
            textRect[i].w = width/15;
            textRect[i].h = height;
        }
        textRect[11].x = 11 * width/15;
        textRect[11].y = 0;
        textRect[11].w = width/15*4;
        textRect[11].h = height;
    }
}