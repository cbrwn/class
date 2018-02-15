#include <cmath>
#include <Windows.h>
#include "ball.h"
#include "game.h"
#include "platform.h"
#include "util.h"

// pass the vector of platforms in from the game class to easily find them
Ball::Ball(std::vector<Platform*>* pforms) : platforms(pforms)
{
	this->x = 32.f;
	this->y = 20.f;
	this->drawY = y;
	this->xvel = 0.0f;
	this->dead = false;

	// make shape (omg I can't draw a circle)
	// (it looks better when it's running trust me)
	this->shape[0] = "  __.__";
	this->shape[1] = " /     \\";
	this->shape[2] = "|       |";
	this->shape[3] = "(       )";
	this->shape[4] = "|       |";
	this->shape[5] = " \\__ __/";
	this->shape[6] = "    `";
}

void Ball::update()
{
	// decrease the animation timer if needed so it will stop eventually
	if (this->bounceAnimTime > 0)
		this->bounceAnimTime--;

	// fall off screen if dead
	if (this->dead)
	{
		if (this->drawY < 70)
			this->drawY += 0.5f;
		return;
	}

	// how high the ball will be at the height of its bounce
	const double bounceHeight = 12.1;
	Platform *close = getClosestPlatform();

	// temp auto control
	this->x -= (this->x - close->x) * 0.12f;

	const float accelSpeed = 0.6f;

	// get x modifier - the direction we want to move
	int mx = 0;
	if (IsKeyDown(VK_LEFT))
		mx -= 1;
	if (IsKeyDown(VK_RIGHT))
		mx += 1;

	// apply acceleration
	if (mx == 0)
	{
		// slow down
		if (fabs(this->xvel) > 0.2f)
			this->xvel *= 0.8;
		else if (fabs(this->xvel) > 0.0f)
			this->xvel = 0.0f;
	}
	else
	{
		this->xvel += mx * accelSpeed;
	}

	// limit velocity
	const float maxVel = 2.0f;
	if (fabs(this->xvel) > maxVel)
		this->xvel = maxVel * (this->xvel < 0 ? -1 : 1);

	// apply velocity
	this->x += this->xvel;

	// grab the distance as a percentage of the spawn distance
	double waveDif = (close->z - this->distBetweenBounce) / PLATFORM_SPAWN_DIST;
	// use sin to make a smooth wave up and then down between each platform
	this->drawY = y - abs(sin(waveDif * 3.14159f) * bounceHeight);

	// try to bounce!
	if (close->z <= PLATFORM_BALL_INTERCEPT)
	{
		// check if the ball hit the platform
		float difx = fabs(this->x - close->x);
		if (difx > 20.0f * close->size)
		{
			this->dead = true;
			globalGame->endGame();
			return;
		}

		// start the bounce animation
		this->bounceAnimTime = MAX_BOUNCE_TIME;
		// flag this platform to say that we've bounced on it
		close->bounced = true;

		globalGame->points++;

		// keep track of how far the next platform is so we can jump smoothly between them
		this->distBetweenBounce = getClosestPlatform()->z;
	}

}

void Ball::draw()
{
	// keep this offset outside of the loop to let us 'cut out' certain rows just by continuing
	// start from (y + height - 1) so the bottom of the ball is at y
	int yofs = BALL_ART_SIZE - 1;
	for (int i = 0; i < BALL_ART_SIZE; i++)
	{
		// first 'frame' of animation - cut out the 3 centermost rows
		if (this->bounceAnimTime >= MAX_BOUNCE_TIME * 0.75f && i > 1 && i < 4)
			continue;
		// second 'frame' - just cut out the middle row
		if (this->bounceAnimTime > 0 && i == 3)
			continue;
		int ypos = (int)(this->drawY - yofs);
		yofs--;
		if (ypos < 0)
			continue;
		if (ypos >= SCREEN_HEIGHT)
			continue;
		SetConsoleCursorPos((int)this->x - 5, ypos);
		printf(this->shape[i]);
	}
}

// returns the platform which is closest to the ball
// used to check if we bounced on a platform
Platform *Ball::getClosestPlatform()
{
	Platform *closest = (*this->platforms)[0];
	for (Platform *p : *this->platforms)
	{
		// make sure we're getting a platform that hasn't been bounced on if possible
		if (closest->bounced && !p->bounced)
			closest = p;
		if (p->z <= closest->z && !p->bounced)
			closest = p;
	}
	return closest;
}