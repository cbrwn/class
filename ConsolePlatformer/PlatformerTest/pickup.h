#pragma once

class Pickup {
public:
	int x, y;
	int anim, animTimer;
	void update();
	void draw();

	Pickup(int x, int y);
};