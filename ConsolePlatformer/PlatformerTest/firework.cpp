#include "firework.h"
#include "game.h"

Firework::Firework()
{
	this->x = 0;
	this->y = 0;
	this->animTime = 0;
	this->frame = 300;
	this->doneTime = 999;

	// firework "frames"
	this->frames[4] = "*       *";
	this->frames[3] = " .  .  .";
	this->frames[2] = "  \\ . /";
	this->frames[1] = "   \\|/";
	this->frames[0] = "    |";
}

void Firework::update()
{
	if (this->frame > MAX_FIREWORK_FRAME) {
		this->doneTime++;
		return;
	}
	this->animTime++;
	if (this->animTime >= 3) {
		this->animTime = 0;
		this->frame++;
	}
}

void Firework::draw()
{
	if (this->frame > MAX_FIREWORK_FRAME)
		return;
	// move the frame upwards as time goes on so it looks like a firework
	int ypos = this->y - this->frame;
	char toPrint[] = "          ";

	globalGame->setCursorPos(this->x - 4, ypos + 1);
	std::cout << toPrint; // clear last frame of animation

	// if we're still in the animation, draw the frame
	// otherwise we want to clear the current position for any stray characters
	if (this->frame <= MAX_FIREWORK_FRAME-1) 
		strcpy_s(toPrint, this->frames[this->frame]);

	globalGame->setCursorPos(this->x - 4, ypos);
	std::cout << toPrint;
}

// moves the firework and begins the animation
void Firework::start(int x, int y)
{
	this->x = x;
	this->y = y;
	this->animTime = 0;
	this->frame = 0;
	this->doneTime = 0;
}
