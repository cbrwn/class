#pragma once

class Player {
public:
	float x, y;
	int lastX, lastY; // used to clear last position
	float vy; // no vx because it's only controlled by input
	int anim, animtimer;
	bool faceLeft;
	bool onGround;
	bool crouch;

	void draw();
	void update();

	Player(float x, float y);
};