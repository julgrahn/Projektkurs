#ifndef BUTTON_H
#define BUTTON_H

#include "sdlinclude.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct Button_Type* Button;
Button createButton(int xButtonPos, int yButtonpos);
SDL_Rect* getButtonRect(Button button);
bool getButtonPressed(Button b);
bool getButtonHovered(Button b);
void setButtonPressed(Button b, bool newValue);
void setButtonHovered(Button b, bool newValue);

//enum ButtonState { NORMAL = 0, HOVER = 1, PRESSED = 2 };





#endif
