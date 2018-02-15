#include <ctime>
#include <Windows.h>
#include "ball.h"
#include "game.h"
#include "platform.h"
#include "util.h"

// this is for easy access from everywhere
// it's probably like really bad
Game *globalGame;

Game::Game() 
{
	srand((unsigned int)time(NULL));
	globalGame = this;

	this->timeSinceClear = 0;
}

Game::~Game() 
{
	delete ball;
	for (Platform *p : platforms)
	{
		delete p;
	}
	platforms.clear();
}


void Game::loop() 
{
	this->running = true;
	this->init();

	while (running) 
	{
		// escape to close game
		if (IsKeyDown(VK_ESCAPE))
			break;

		this->update();

		this->draw();
		
		// makes it run slower (in a good way) and flicker less
		Sleep(25);
	}
}

void Game::init()
{
	// these platforms will loop
	this->platforms.push_back(new Platform(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	this->platforms.push_back(new Platform(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, PLATFORM_SPAWN_DIST));

	this->ball = new Ball(&this->platforms);
	this->ball->x = SCREEN_WIDTH / 2;
}

void Game::update()
{
	// todo - control platform movement here instead
	for (Platform *p : platforms)
	{
		p->update();
	}

	ball->update();

	// this will be used once I stop using cls to clear the screen
	//    to clean up any characters I missed
	/*this->timeSinceClear++;
	if (this->timeSinceClear >= 100) {
		this->timeSinceClear = 0;
		system("cls");
	}*/
}

void Game::draw()
{
	system("cls");
	for (Platform *p : platforms)
	{
		p->draw();
	}
	ball->draw();
}