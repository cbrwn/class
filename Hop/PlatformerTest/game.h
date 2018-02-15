#pragma once

#include <iostream>
#include <vector>

#define SCREEN_WIDTH 79
#define SCREEN_HEIGHT 25

class Ball;
class Game; // declare this here so it's easier to find globalGame
class Platform;

// see my apology in game.cpp
extern Game *globalGame;

class Game {
public:
	bool running;
	int timeSinceClear;

	// this game's specific stuff
	Ball *ball;
	std::vector<Platform *> platforms;

	// game stuff
	void loop();

	void init();
	void update();
	void draw();

	Game();
	~Game();
};