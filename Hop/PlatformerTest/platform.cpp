#include <iostream>
#include <cmath>
#include "game.h"
#include "platform.h"
#include "util.h"

Platform::Platform(float x, float y, float z) : x(x), y(y), z(z)
{
	this->spd = -0.005;
	this->bounced = false;
}

void Platform::update()
{
	this->z += this->spd;

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
	int drawWidth = (int)(((3 / (z / 15.f)) * 0.03f) * 0.5f);

	// limit width because it can get crazy when very close to the camera
	const int maxDrawWidth = 60;
	if (drawWidth > maxDrawWidth)
	{
		drawWidth = maxDrawWidth;
		return;
	}

	// vertical difference between the front edge and the back edge
	int backDrawHeight = (int)(drawWidth * 0.1f);
	// make sure the back line is always shown
	if (backDrawHeight < 1)
		backDrawHeight = 1;

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

	// width of the back edge
	int backDrawWidth = (int)(drawWidth * 0.7f);

	// x positions of corners
	int frontRight = frontLeft + drawWidth;
	int backLeft = (int)this->x - backDrawWidth / 2;
	int backRight = backLeft + backDrawWidth;

	// y position of the front edge
	int frontY = (int)this->y + (drawWidth/6) + 1;

	// how thick (tall?) the platform should be drawn
	int visibleHeight = drawWidth / 15;

	// draw front of platform
	drawHorizontalLine(frontLeft, frontY, drawWidth); // top edge
	drawHorizontalLine(frontLeft, frontY + visibleHeight, drawWidth); // bottom edge
	// and back of platform
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
}
