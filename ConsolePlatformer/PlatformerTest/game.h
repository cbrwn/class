#pragma once

#include <iostream>
#include <vector>

#define SCREEN_WIDTH 79
#define SCREEN_HEIGHT 25

class Firework;
class Game; // declare this here so it's easier to find globalGame
class Pickup;
class Platform;
class Player;

// see my apology in game.cpp
extern Game *globalGame;

class Game {
public:
	bool running;
	int timeSinceClear;

	int score;
	int timeLeft;

	// entities
	Player *plr;
	std::vector<Platform *> platforms;
	Pickup *pickup;
	Firework *firework;

	// game stuff
	void loop();

	void init();
	void update();
	void draw();

	// game-specific stuff
	void movePickup();

	// misc util functions
	void printCenter(const char s[], int y);
	void setCursorPos(int x, int y);
	bool keyDown(int id);

	Game();
	~Game();
};