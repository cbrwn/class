#include <ctime>
#include <Windows.h>
#include "firework.h"
#include "game.h"
#include "pickup.h"
#include "platform.h"
#include "player.h"

// this is for easy access from everywhere
// it's probably like really bad
Game *globalGame;

Game::Game() 
{
	srand((unsigned int)time(NULL));
	globalGame = this;

	// make player
	this->plr = new Player(SCREEN_WIDTH / 2.f, 3);

	// make platforms
	Platform *p = new Platform(1, SCREEN_HEIGHT-1, SCREEN_WIDTH-1); // ground platform
	this->platforms.push_back(p);

	this->platforms.push_back(new Platform(10, SCREEN_HEIGHT - 4, 10));
	this->platforms.push_back(new Platform(18, SCREEN_HEIGHT - 8, 10));
	this->platforms.push_back(new Platform(SCREEN_WIDTH - 20, SCREEN_HEIGHT - 4, 10));
	this->platforms.push_back(new Platform(SCREEN_WIDTH - 28, SCREEN_HEIGHT - 8, 10));
	this->platforms.push_back(new Platform(SCREEN_WIDTH/2 - 5, SCREEN_HEIGHT - 12, 10));
	this->platforms.push_back(new Platform(24, SCREEN_HEIGHT - 16, 4));
	this->platforms.push_back(new Platform(SCREEN_WIDTH - 28, SCREEN_HEIGHT - 16, 4));

	this->pickup = nullptr; // is created in movePickup()
	this->firework = new Firework();

	this->timeSinceClear;
}

Game::~Game() 
{
	delete plr;
	delete firework;
	if(pickup != nullptr)
		delete pickup;
	for (auto p : this->platforms)
		delete p;
}


void Game::loop() 
{
	this->running = true;
	this->init();

	while (running) 
	{
		// escape to close game
		if (this->keyDown(VK_ESCAPE))
			break;

		this->update();

		this->draw();
		
		// makes it run slower (in a good way) and flicker less
		Sleep(25);
	}
}

void Game::init()
{
	this->score = 0;
	this->timeLeft = 60 * 30;

	// make sure pickup is somewhere random
	movePickup();

	// reset player position
	this->plr->x = SCREEN_WIDTH / 2.f;
	this->plr->y = 3;

	// stop firework animation if it was playing
	this->firework->frame = 999; 
}

void Game::update()
{
	pickup->update();
	firework->update();

	if (this->timeLeft > 0)
	{
		// stop updating the player so they can't move anymore
		// could check for game over in player->update tbh
		plr->update();
		this->timeLeft--;
	}
	else
	{
		// make pretty fireworks yay
		if (firework->doneTime >= 6) 
			firework->start((rand() % SCREEN_WIDTH), (rand() % 9) + 4);

		// enter to restart
		if (this->keyDown(VK_RETURN))
			this->init();
	}

	this->timeSinceClear++;
	if (this->timeSinceClear >= 100) {
		this->timeSinceClear = 0;
		system("cls");
	}
}

void Game::draw()
{
	// draw all the things
	for (auto p : platforms)
		p->draw();
	pickup->draw();
	firework->draw();
	plr->draw();

	if (this->timeLeft > 0) 
	{
		// ui while playing
		this->setCursorPos(3, 2);
		std::cout << "Score: " << this->score;
		this->setCursorPos(3, 4);
		std::cout << "Time:  " << this->timeLeft/30 << "  ";
	}
	else
	{
		// game over screen
		this->printCenter("Game over!", 4);
		char gameOverMessage[128];
		sprintf_s(gameOverMessage, "You scored %d points!", this->score);
		this->printCenter(gameOverMessage, 6);
		this->printCenter("Press enter to play again", 10);
	}
}

// moves the pickup to a random platform
// used when collecting the pickup to move it somewhere else
void Game::movePickup()
{
	if (this->pickup == nullptr)
		this->pickup = new Pickup(0, 0);

	this->setCursorPos(this->pickup->x, this->pickup->y);
	std::cout << ' ';

	// grab a random platform
	Platform *spawnPlatform = platforms[(rand() % platforms.size())];
	this->pickup->x = (int)spawnPlatform->x + (rand() % (int)spawnPlatform->w);
	this->pickup->y = (int)spawnPlatform->y-1;
}

// prints a string s in the center of the screen
void Game::printCenter(const char s[], int y)
{
	this->setCursorPos(SCREEN_WIDTH / 2 - (strlen(s) / 2), y);
	std::cout << s;
}

void Game::setCursorPos(int x, int y)
{
	HANDLE win = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (short)x, (short)y };
	SetConsoleCursorPosition(win, pos);
}

bool Game::keyDown(int id)
{
	short kstate = GetKeyState(id);
	// key state will be negative if it's down
	return kstate < 0;
}
