#include <stdio.h>
#include "sdlinclude.h"
#include <stdbool.h>
#include <stdlib.h>
#include "player.h"
#include "world.h"
#include "bullet.h"
#include "server.h"
#include "networkgamestate.h"
#include "initFunctions.h"
#include "clientNetFunctions.h"
#include "gameFunctions.h"
#include "renderFunctions.h"

SDL_mutex* mutex;


void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting, bool* newGame, bool *reload);
void startPrompt(int* playerID, Server* server, bool* host);
void startNewGame(TCPsocket* tcpsock);
void handleClientTCP(TCPsocket* tcpsock, SDLNet_SocketSet* set, Networkgamestate networkgamestate, Player players[], int playerID);

int main(int argc, char* args[])
{
    // Variables

    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    UDPsocket sd;
    TCPsocket tcpsock = NULL;
    SDLNet_SocketSet set;
    IPaddress srvadd;
    UDPpacket* p;
    UDPpacket* p2;
    Server server = NULL;
    int playerID;
    SDL_Cursor* cursor = NULL;
    Player players[MAX_PLAYERS];
    SDL_Texture* playerText;
    SDL_Rect playerRect[4];
    int mouseX = 0, mouseY = 0;
    Bullet bullets[MAX_PLAYERS][MAX_BULLETS];
    SDL_Texture* tiles = NULL;
    SDL_Rect gridTiles[900];   // Kommer innehålla alla 900 rutor från bakgrundsbilden, kan optmiseras.
    bool isPlaying = true, shooting = false, host = false, connected = false, newGame = false, reload = false;
    SDL_Texture* bulletTexture = NULL;
    SDL_Texture* gunFireTexture = NULL;
    SDL_Rect gunFireRect;
    Mix_Chunk* sound;
    gunFireRect.w = 40;
    gunFireRect.h = 40;
    SDL_Texture* bloodTexture = NULL;
    SDL_Rect bloodRect;
    bloodRect.w = 64;
    bloodRect.h = 64;
    SDL_Rect bloodTiles[48];
    SDL_Texture* explosionTexture = NULL;
    SDL_Rect explosionRect;
    explosionRect.w = 40;
    explosionRect.h = 40;
    SDL_Texture *textTexture;
    SDL_Rect textRect[15];
    SDL_Rect healthBar;
    SDL_Rect reloadTimer;
    SDL_Rect explosionTiles[121]; // Rutor från explosions.png
    int up = 0, down = 0, left = 0, right = 0;
    SDL_Point playerRotationPoint = { 20, 32 };
    SDL_Point muzzleRotationPoint = { 14, 16 };
    Networkgamestate networkgamestate = createNetworkgamestate();
    Button buttons[3];
    SDL_Texture* connectTextures[3];
    SDL_Texture* hostTextures[3];
    SDL_Texture* quitTextures[3];

    // Init functions
    set = SDLNet_AllocSocketSet(1);
    mutex = SDL_CreateMutex();
    if (!initSDL(&renderer, &sound)) return 1;
    initGameObjects(players, bullets);
    initGameHUD(renderer, textRect, &textTexture, &healthBar, &reloadTimer);
    loadMenu(renderer, connectTextures, hostTextures, quitTextures);
    initClient(&sd, &p, &p2);
    loadMedia(renderer, gridTiles, &tiles, playerRect, &playerText, &cursor, &bulletTexture, 
            &gunFireTexture, &explosionTexture, &bloodTexture, 
            &sound, explosionTiles, bloodTiles);

    // Menu loop
    buttons[0] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, CONNECT_Y_POS);
    buttons[1] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, HOST_Y_POS);
    buttons[2] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, QUIT_Y_POS);
    while (isPlaying && !connected)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouseX, &mouseY, &shooting, &newGame, &reload);
        renderMenu(renderer, connectTextures, hostTextures, quitTextures, buttons, mouseX, mouseY, shooting);

        if (mouseX >= (WINDOWWIDTH / 2) - BUTTON_HEIGHT && mouseX <= (WINDOWWIDTH / 2) + BUTTON_HEIGHT)
        {
            // Connect button
            if (mouseY > CONNECT_Y_POS && mouseY < CONNECT_Y_POS + BUTTON_HEIGHT && shooting)
            {
                setButtonPressed(buttons[0], true);
                connectToServer(LOCAL_IP, &srvadd, &tcpsock, networkgamestate, &playerID, players, &sd, &connected);
                startUDPreceiveThread(&sd, &p2, bullets, players, &networkgamestate, playerID, &mutex);
                SDLNet_TCP_AddSocket(set, tcpsock);
            }

            // Host button
            if (mouseY > HOST_Y_POS && mouseY < HOST_Y_POS + BUTTON_HEIGHT && shooting)
            {
                server = createServer();
                startServer(server);
                host = true;
                setButtonPressed(buttons[1], true);
                connectToServer(LOCAL_IP, &srvadd, &tcpsock, networkgamestate, &playerID, players, &sd, &connected);
                startUDPreceiveThread(&sd, &p2, bullets, players, &networkgamestate, playerID, &mutex);
                SDLNet_TCP_AddSocket(set, tcpsock);
            }

            // Quit button
            if (mouseY > QUIT_Y_POS && mouseY < QUIT_Y_POS + BUTTON_HEIGHT && shooting)
            {
                setButtonPressed(buttons[2], true);
                isPlaying = false;
            }
        }
    }
    // Main loop
    while (isPlaying)
    {
        playerTick(players[playerID]);
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouseX, &mouseY, &shooting, &newGame, &reload);
        if (newGame && host)
        {
            startNewGame(&tcpsock);
            newGame = false;
        }
        setPlayerShooting(players[playerID], shooting, mouseX, mouseY);
        if (isPlayerAlive(players[playerID]))
        {
            movePlayer(players[playerID], up, down, right, left, mouseX, mouseY, reload);
            if (isPlayershooting(players[playerID])) fire(bullets[playerID], players[playerID], playerID);
        }
        //Flytta på alla andra spelare
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (i != playerID) {
                moveOtherPlayers(players[i]);
            }
        }
        simulateBullets(bullets);
        SDL_LockMutex(mutex);
        setNetPlayer(networkgamestate, playerID, players[playerID]);
        setNetBullets(networkgamestate, playerID, bullets[playerID]);
        sendUDP(getNetPlayer(networkgamestate, playerID), &sd, &srvadd, &p, &p2);
        handleClientTCP(&tcpsock, &set, networkgamestate, players, playerID);
        SDL_UnlockMutex(mutex);

        renderGame(renderer, tiles, gridTiles, bullets, bulletTexture, players, playerText, 
                    playerRect, &playerRotationPoint, gunFireTexture, gunFireRect, 
                    explosionTexture, explosionRect,  &muzzleRotationPoint, bloodTexture, 
                    bloodRect, sound, explosionTiles, bloodTiles);
        renderHUD(renderer, players[playerID], textRect, textTexture, &healthBar, &reloadTimer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window); // beh�vs denna?
    SDL_Quit();
    return 0;
}

void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting, bool* newGame, bool *reload)
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
            case SDL_SCANCODE_P:
                *newGame = true;
                break;
            case SDL_SCANCODE_R:
                *reload = true;
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
            case SDL_SCANCODE_R:
                *reload = false;
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


// Ska ersättas med menyn
void startPrompt(int* playerID, Server* server, bool* host)
{
    printf("Host(h) or client(c): ");
    char input;
    scanf(" %c", &input);
    if (input == 'h')
    {
        printf("hosted!\n");
        *host = true;
    }
}

