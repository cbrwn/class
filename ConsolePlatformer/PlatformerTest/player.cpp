#include <Windows.h>
#include "firework.h"
#include "game.h"
#include "pickup.h"
#include "platform.h"
#include "player.h"

Player::Player(float x, float y) : x(x), y(y)
{
	this->vy = 0;
	this->onGround = false;
	this->faceLeft = false;
	this->anim = 0;
	this->animtimer = 0;
}

void Player::draw() 
{
	// different y position if we're crouching - draw head where body would be
	globalGame->setCursorPos((int)this->x, (int)this->y - (crouch ? 0 : 1));

	// show which way we're facing
	char pimg = this->faceLeft ? '<' : '>';
	std::cout << pimg;

	// don't print the body if we're crouching - the head is drawn here!
	if (!crouch) 
	{
		globalGame->setCursorPos((int)this->x, (int)this->y);
		char body = 'X';

		if (this->onGround)
		{
			// change to Y so it looks like the legs are crossing
			if (this->anim == 1)
				body = 'Y';
		}
		else 
		{
			// attempted to make it look kinda floppy when you're in the air
			// but it just likes someone trying to naruto run in mid air
			body = this->faceLeft ? '\\' : '/';
		}
		std::cout << body;
	}
	else
	{
		globalGame->setCursorPos((int)this->x, (int)this->y - 1);
		std::cout << ' ';
	}
}

void Player::update()
{
	char mx = 0;
	if (globalGame->keyDown(VK_LEFT))
		mx--;
	if (globalGame->keyDown(VK_RIGHT))
		mx++;

	// hardcode the bottom of the screen so we can't fall through the ground platform
	if (globalGame->keyDown(VK_DOWN) && this->onGround && this->y < SCREEN_HEIGHT - 2) 
		this->y += 1.5f;
	
	if (globalGame->keyDown(VK_UP) && this->onGround)
		this->vy = -1.0f;

	this->crouch = globalGame->keyDown(VK_DOWN);

	this->x += mx / (this->crouch ? 4.f : 1.f);


	if (mx != 0) 
	{
		this->faceLeft = mx < 0;
		this->animtimer += 1;
		if (this->animtimer >= 3) 
		{
			this->animtimer = 0;
			this->anim = (this->anim + 1) % 2;
		}
	} 
	else
	{
		this->anim = 0;
	}

	this->vy += 0.1f;

	// check for platform collisions throughout the movement
	this->onGround = false;
	for (auto p : globalGame->platforms) 
	{
		// check if it's even possible for it to have collided
		if (this->x < p->x || this->x > p->x + p->w)
			continue;
		if (p->y >= this->y && p->y <= this->y + this->vy) 
		{
			// should have collided!
			this->y = p->y - 1;
			this->vy = 0;
			this->onGround = true;
			break;
		}
	}

	//check if we're on the ground if not already
	if (!this->onGround) 
	{
		for (auto p : globalGame->platforms)
		{
			// check if platform is directly below player
			if (this->x < p->x || this->x > p->x + p->w || (int)this->y + 1 != (int)p->y)
				continue;
			this->onGround = true;
			break;
		}
	}

	this->y += this->vy;

	if (lastY != (int)this->y || lastX != (int)this->x) {
		globalGame->setCursorPos(lastX, lastY);
		std::cout << ' ';
		globalGame->setCursorPos(lastX, lastY-1);
		std::cout << ' ';
	}

	// keep us on the screen because it gets all weird and scrolly when you go off it
	if (this->x <= 0)
		this->x = 1;
	if (this->x >= SCREEN_WIDTH)
		this->x = SCREEN_WIDTH - 1;
	if (this->y < 0)
		this->y = 0;
	if (this->y >= SCREEN_HEIGHT)
		this->y = SCREEN_HEIGHT - 2;

	// check for pickup collision - 2 y position checks to account for our head
	if ((int)this->x == globalGame->pickup->x && 
		((int)this->y == globalGame->pickup->y || (int)this->y-1 == globalGame->pickup->y)) 
	{
		globalGame->movePickup();
		globalGame->score++;
		globalGame->firework->start((int)this->x, (int)this->y - 2);
	}

	this->lastX = (int)this->x;
	this->lastY = (int)this->y;
}