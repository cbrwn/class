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
		delete p;
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
		this->clearObjects();
		this->draw();
		
		// makes it run slower (in a good way) and flicker less
		Sleep(20);
	}
}

void Game::init()
{
	// clear everything - for restarting, not so much for the beginning of the program
	system("cls");

	// these platforms will loop
	this->platforms.push_back(new Platform(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 2, 0.0f));
	this->platforms.push_back(new Platform(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 2, PLATFORM_SPAWN_DIST));

	this->ball = new Ball(&this->platforms);
	this->ball->x = SCREEN_WIDTH / 2;

	this->points = 0;
	this->platformSpeed = 0.005f;

	this->gameStarted = false;
	this->gameOver = false;
	this->gameOverTimer = 0;
}

void Game::update()
{
	if (IsKeyDown(VK_SPACE))
	{
		// clear the starting text (badly)
		SetConsoleCursorPos(2, 2);
		printf("                     ");
		SetConsoleCursorPos(2, 4);
		printf("                           ");
		SetConsoleCursorPos(2, 5);
		printf("                       ");

		// start the game :D
		this->gameStarted = true;
	}

	if (!this->gameStarted)
		return;

	for (Platform *p : platforms)
	{
		p->update();

		// move platforms here so we can control the speed easily
		p->z -= this->platformSpeed;
	}

	ball->update();

	// update game over screen
	if (this->gameOver)
	{
		if (this->gameOverTimer > 0)
			this->gameOverTimer--;

		// restart
		if (IsKeyDown(VK_RETURN))
			this->startOver();
	}

	// use cls occasionally to clear anything that was missed by clearObjects
	this->timeSinceClear++;
	if (this->timeSinceClear >= 100) {
		this->timeSinceClear = 0;
		system("cls");
	}
}

void Game::draw()
{
	for (Platform *p : platforms)
		p->draw();
	ball->draw();

	if (!this->gameStarted)
	{
		SetConsoleCursorPos(2, 2);
		printf("Press space to start!");
		SetConsoleCursorPos(2, 4);
		printf("Use the arrow keys to move,");
		SetConsoleCursorPos(2, 5);
		printf("don't miss a platform!");
	}

	// draw game over screen
	if (this->gameOver && this->gameOverTimer <= 0)
	{
		SetConsoleCursorPos(2, 2);
		printf("game over");
		SetConsoleCursorPos(2, 4);
		printf("press enter to restart");
	}

	// score display
	SetConsoleCursorPos(SCREEN_WIDTH / 2 - 1, 3);
	char pointString[6] = "";
	sprintf(pointString, "%d ", this->points);
	PrintCenter(pointString, 3);
}

// called when the player dies
void Game::endGame()
{
	this->gameOver = true;
	this->platformSpeed = 0.0f;

	// this timer controls when the game over screen appears
	// set to 30 so it appears when the ball falls off screen
	this->gameOverTimer = 30;
}

// destroy everything and restart the game
void Game::startOver()
{
	this->~Game();
	this->init();
}

void Game::clearObjects()
{
	for (Platform *p : platforms)
		p->clear();
	ball->clear();
}