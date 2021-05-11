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
#include "initFunctions.h"
#include "clientNetFunctions.h"
#include "gameFunctions.h"
#include "renderFunctions.h"
#include "button.h"


SDL_mutex *mutex;

void handleEvents(SDL_Event* event, int* up, int* down, int* right, int* left, bool* isPlaying, int* mouseX, int* mouseY, bool* shooting);
void startPrompt(int* playerID, Server* server, bool* host);

int main(int argc, char* args[])
{ 
    // Variables
    SDL_Point playerRotationPoint = { 20, 32 };
    SDL_Event event;
    SDL_Renderer* renderer = NULL;
    UDPsocket sd;
    TCPsocket tcpsock = NULL;
    IPaddress srvadd;
    UDPpacket* p;
    UDPpacket* p2;
    Server server = NULL;
    SDL_Cursor* cursor = NULL;
    Player players[MAX_PLAYERS];
    Button buttons[3];
    SDL_Rect playerRect[4];
    Bullet bullets[MAX_BULLETS];
    SDL_Rect gridTiles[900];   // Kommer innehålla alla 900 rutor från bakgrundsbilden, kan optmiseras.
    Networkgamestate networkgamestate = createNetworkgamestate();
    SDL_Texture* bulletTexture = NULL;
    SDL_Texture* connectTextures[3];
    SDL_Texture* hostTextures[3];
    SDL_Texture* quitTextures[3];
    SDL_Texture* tiles = NULL;
    SDL_Texture* playerText;


    int up = 0, down = 0, left = 0, right = 0;
    int playerID = 1;
    int mouseX = 0, mouseY = 0;
    bool isPlaying = true, shooting = false, host = false, connected = false;

    

    // Init functions
    mutex = SDL_CreateMutex();
    if (!initSDL(&renderer)) return 1;
    initGameObjects(players, bullets);
    loadMenu(renderer, &connectTextures, &hostTextures, &quitTextures);
    
    initClient(&sd, &p, &p2); 
    loadMedia(renderer, gridTiles, &tiles, playerRect, &playerText, &cursor, &bulletTexture);  


    //Menu loop
    buttons[0] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, CONNECT_Y_POS);
    buttons[1] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, HOST_Y_POS);
    buttons[2] = createButton((WINDOWWIDTH / 2) - BUTTON_HEIGHT, QUIT_Y_POS);
    while (isPlaying && !connected)
    {
        handleEvents(&event, &up, &down, &right, &left, &isPlaying, &mouseX, &mouseY, &shooting);
        renderMenu(renderer, connectTextures, hostTextures, quitTextures, buttons, mouseX, mouseY, shooting);

        
        if (mouseX >= (WINDOWWIDTH / 2) - BUTTON_HEIGHT && mouseX <= (WINDOWWIDTH / 2) + BUTTON_HEIGHT)
        {
            // Connect button

            if (mouseY > CONNECT_Y_POS && mouseY < CONNECT_Y_POS + BUTTON_HEIGHT && shooting)
            {
                setButtonPressed(buttons[0], true);
                connectToServer(LOCAL_IP, &srvadd, &tcpsock, networkgamestate, &playerID, players, &sd, &connected);
                startUDPreceiveThread(&sd, &p2, bullets, players, &networkgamestate, playerID, &mutex);
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
    while (isPlaying && connected)
    {
        SDL_Delay(1000 / 60);
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
        playerBulletCollisionCheck(bullets, players);
        SDL_LockMutex(mutex);
        setNetworkgamestateplayer(&networkgamestate, playerID, players[playerID]);
        sendUDP(getNetworkgamestateplayer(&networkgamestate, playerID), &sd, &srvadd, &p, &p2);
        SDL_UnlockMutex(mutex);
        renderGame(renderer, tiles, gridTiles, bullets, bulletTexture, players, playerText, playerRect, &playerRotationPoint);
    }

    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
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

