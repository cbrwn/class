#pragma once

#define PLATFORM_SPAWN_DIST (0.15f)
#define PLATFORM_BALL_INTERCEPT (0.04f)

class Platform {
private:
	void drawHorizontalLine(int xpos, int ypos, int length);

	Platform *getFurthestPlatform();
public:
	// position
	double x, y, z;
	double spd;
	bool bounced;

	void reset();

	void update();
	void draw();

	Platform(float x, float y, float z);
};