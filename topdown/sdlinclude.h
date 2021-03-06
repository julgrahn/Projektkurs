#ifndef SDLINCLUDE_H
#define SDLINCLUDE_H

/****************
* What the numbers in the TCP-packets mean:
* 
* Message from host to server
* 1: Start a new game
* 2: Start warmup
*********************/


 #include <SDL.h>
 #include <SDL_image.h>
 #include <SDL_timer.h>
 #include <SDL_net.h>
 #include <SDL_mixer.h>
 #include <SDL_ttf.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_timer.h>
//#include <SDL2/SDL_net.h>
//#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL_ttf.h>


#define WINDOWWIDTH 1152
#define WINDOWHEIGHT 768
#define MAX_BULLETS 9
#define BUTTON_HEIGHT 64
#define CONNECT_Y_POS 100
#define HOST_Y_POS 200
#define QUIT_Y_POS 300
#define ANDREAS_IP "78.71.16.247"
#define ALEX_IP "178.78.213.173"
#define JOHAN_IP "188.149.151.240"
#define LOCAL_IP "127.0.0.1"
#define MAX_PLAYERS 5
#define START_LIVES 3
#define STANDARDSPAWNTIME 180

#endif