#include "game.h"
#include "pickup.h"

Pickup::Pickup(int x, int y) : x(x), y(y)
{ 
	this->anim = 0;
	this->animTimer = 0;
}

void Pickup::update()
{
	this->animTimer++;
	if (this->animTimer >= 3) {
		this->animTimer = 0;
		this->anim = (this->anim + 1) % 2;
	}
}

void Pickup::draw()
{
	// make pickup twinkle a little bit
	char img = '*';
	if (this->anim == 1)
		img = '#';
	globalGame->setCursorPos(this->x, this->y);
	std::cout << img;
}
