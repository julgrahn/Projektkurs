#include "sdlinclude.h"
#include "button.h"
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC

//enum ButtonState { NORMAL = 0, HOVER = 1, PRESSED = 2 };


/*struct Bullet_type {
	SDL_Rect dimensions;
	bool active;
	int speed;
	double xPos;
	double yPos;
	double xSpeed;
	double ySpeed;
	int owner;
	double direction;
	int damage;
};

PUBLIC Bullet createBullet()
{
	Bullet b = malloc(sizeof(struct Bullet_type));
	b->active = false;
	b->dimensions.w = 4;
	b->dimensions.h = 4;
	b->speed = BULLET_SPEED;
	b->damage = BULLET_DAMAGE;

	return b;
}*/

struct Button_Type
{
	SDL_Rect dimensions;
	bool isPressed;
	bool isHovered;
	
};

PUBLIC Button createButton(int xButtonPos, int yButtonPos)
{
	Button b = malloc(sizeof(struct Button_Type));
	b->dimensions.w = 128;
	b->dimensions.h = 64;
	b->dimensions.x = xButtonPos;
	b->dimensions.y = yButtonPos;
	b->isPressed = false;
	b->isHovered = false;

	return b;
}

PUBLIC SDL_Rect* getButtonRect(Button button)
{
	return &button->dimensions;
}

PUBLIC bool getButtonPressed(Button b)
{
	return b->isPressed;
}

PUBLIC bool getButtonHovered(Button b)
{
	return b->isHovered;
}

PUBLIC void setButtonPressed(Button b, bool newValue)
{
	b->isPressed = newValue;
}

PUBLIC void setButtonHovered(Button b, bool newValue)
{
	b->isHovered = newValue;
}

//public void buttonclick
// exit game