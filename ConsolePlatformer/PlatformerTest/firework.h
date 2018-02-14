#pragma once

#define MAX_FIREWORK_FRAME 5

class Firework {
public:
	int x, y;
	const char *frames[5];
	int frame, animTime;
	int doneTime; // the number of frames the animation has been finished for

	void update();
	void draw();

	void start(int x, int y);

	Firework();
};