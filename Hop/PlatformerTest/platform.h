#pragma once

#include "util.h"

#define PLATFORM_SPAWN_DIST (0.17f)
#define PLATFORM_BALL_INTERCEPT (0.02f)

class Platform {
private:
	Rect clearRect;

	void drawHorizontalLine(int xpos, int ypos, int length);

	Platform *getFurthestPlatform();
public:
	// position
	float x, y, z;
	float size;
	bool bounced;

	void reset();

	void update();
	void clear();
	void draw();

	Platform(float x, float y, float z);
};