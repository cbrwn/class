#pragma once

#include <vector>

#define BALL_ART_SIZE (7)

// time (in frames) of how long the bounce 'animation' should last
#define MAX_BOUNCE_TIME (5)

class Platform;

class Ball
{
private:
	int bounceAnimTime;

	float drawY;
	float distBetweenBounce;
	const char *shape[BALL_ART_SIZE];

	std::vector<Platform *> *platforms;

	Platform *getClosestPlatform();
	void tryBounce();
public:
	float x, y;

	void update();
	void draw();

	Ball(std::vector<Platform *> *pforms);
};