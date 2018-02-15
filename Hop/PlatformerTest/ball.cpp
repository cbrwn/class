#include <cmath>
#include "ball.h"
#include "platform.h"
#include "util.h"

// pass the vector of platforms in from the game class to easily find them
Ball::Ball(std::vector<Platform*>* pforms) : platforms(pforms)
{
	this->x = 32.f;
	this->y = 18.f;
	this->drawY = y;

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
	if (this->bounceAnimTime > 0)
		this->bounceAnimTime--;

	// how high the ball will be at the height of its bounce
	const double bounceHeight = 13.1;
	Platform *close = getClosestPlatform();

	this->x -= (this->x - close->x) * 0.09f;

	double waveDif = (close->z - this->distBetweenBounce + 0.01f) / PLATFORM_SPAWN_DIST;
	this->drawY = y - abs(sin(waveDif * 3.14159f) * bounceHeight);

	if (close->z <= PLATFORM_BALL_INTERCEPT)
	{
		this->bounceAnimTime = MAX_BOUNCE_TIME;
		close->bounced = true;

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

void Ball::tryBounce()
{
}
