#include <iostream>
#include <cmath>
#include "game.h"
#include "platform.h"
#include "util.h"

Platform::Platform(float x, float y, float z) : x(x), y(y), z(z)
{
	this->size = 1.0;
	this->bounced = false;
}

void Platform::update()
{
	// make sure size doesn't go too low
	const double minSize = 0.3;
	if (this->size <= minSize)
		this->size = minSize;

	// send the platform back to be used again if it's past the camera
	if (this->z < 0.0)
		this->reset();
}

void Platform::draw()
{
	// don't draw if it's past where our view would be
	if (this->z <= 0.0f)
		return;

	// I have no idea how I got these numbers apart from playing around
	// the width increases exponentially as the z value gets lower
	// lower z = closer to camera = larger object
	float drawWidth = (2.0f / ((z) / 15.0f)) * 0.04f * this->size;

	float distInverse = (2.0f / ((z) / 15.0f)) * 0.04;

	if (this->z < 0.019f)
	{
		return;
	}

	// vertical difference between the front edge and the back edge
	int backDrawHeight = (int)(distInverse * 0.08f);

	// calculate corners of platform
	int frontLeft = (int)this->x - drawWidth / 2;

	// adjust values for if the platform is to be drawn off screen
	if (frontLeft < 0)
	{
		drawWidth += frontLeft;
		frontLeft = 0;
	}
	if (frontLeft + drawWidth >= SCREEN_WIDTH)
		drawWidth = (SCREEN_WIDTH - frontLeft) - 1;

	// y position of the front edge
	int frontY = (int)this->y + (distInverse/6) + 1;

	// width of the back edge
	int backDrawWidth = (int)(drawWidth - backDrawHeight - 4);

	// x positions of corners
	int frontRight = frontLeft + drawWidth;
	int backLeft = (int)this->x - backDrawWidth / 2 - 1;
	int backRight = backLeft + backDrawWidth;

	// how thick (tall?) the platform should be drawn
	int visibleHeight = distInverse / 17;

	// draw front of platform
	drawHorizontalLine(frontLeft, frontY, drawWidth); // top edge
	drawHorizontalLine(frontLeft, frontY + visibleHeight, drawWidth); // bottom edge
	// and back of platform if necessary
	if(backDrawHeight >= 1.0f)
		drawHorizontalLine(backLeft, frontY - backDrawHeight, backDrawWidth);

	// draw vertical lines connecting the top and bottom edges if there's room
	if (visibleHeight >= 1)
	{
		for (int i = 1; i < visibleHeight; i++)
		{
			SetConsoleCursorPos(frontLeft, frontY + i);
			printf("|");
			SetConsoleCursorPos(frontRight-1, frontY + i);
			printf("|");
		}
	}

	// no need for connecting lines if the lines we're connecting are only 1 line apart
	if (backDrawHeight <= 1)
		return;

	// start drawing diagonal conecting lines

	// getting the angle between the corners didn't work well and an angle
	//   of 0 seems to work okay
	float ang = 0.0f;
	// draw edges
	for (int i = 1; i <= backDrawHeight; i++)
	{
		// left edge
		int drawX = (int)(cosf(ang) * i);
		SetConsoleCursorPos(frontLeft + drawX, frontY - i);
		printf("/");

		drawX = (int)(cosf(ang + 3.14159f) * i);
		SetConsoleCursorPos(frontRight + drawX - 1, frontY - i);
		printf("\\");
	}
}

// draws a horizontal line of = signs starting from xpos
void Platform::drawHorizontalLine(int xpos, int ypos, int length)
{
	// don't draw anything if it's off the screen vertically
	if (ypos < 0 || ypos >= SCREEN_HEIGHT - 1)
		return;

	// check if this line will go off the screen and adjust it if so
	if (xpos + length > SCREEN_WIDTH)
		length = SCREEN_WIDTH - xpos - 1;

	SetConsoleCursorPos(xpos, ypos);
	for (int i = 0; i < length; i++)
		printf("=");
}

// gets the platform furthest away from the camera
// useful for setting a position relative to the furthest platform when reseting
Platform *Platform::getFurthestPlatform()
{
	Platform *furthest = this;
	for (Platform *p : globalGame->platforms)
		if (p->z > furthest->z)
			furthest = p;
	return furthest;
}

// sends this platform behind all other platforms, changes its x position and resets its bounced state
// used to reuse a platform after it has moved too far
void Platform::reset()
{
	this->bounced = false;
	// 20 < x < (screen width - 40)
	this->x = 20 + (rand() % (int)(SCREEN_WIDTH - 40));
	// make sure it's a set distance away from the furthest platform instead of
	//    the camera so the gaps between the platforms stay consistent
	this->z = this->getFurthestPlatform()->z + PLATFORM_SPAWN_DIST;

	// increase difficulty slightly
	this->size -= 0.02;
	globalGame->platformSpeed += 0.0001f;
}
