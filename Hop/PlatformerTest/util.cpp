#include <iostream>
#include <Windows.h>
#include "game.h"
#include "util.h"

// prints a string s in the center of the screen
void PrintCenter(const char s[], int y)
{
	SetConsoleCursorPos(SCREEN_WIDTH / 2 - ((int)strlen(s) / 2), y);
	std::cout << s;
}

// sets the console cursor position to x,y
void SetConsoleCursorPos(int x, int y)
{
	HANDLE win = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (short)x, (short)y };
	SetConsoleCursorPosition(win, pos);
}

// returns true if key with this id is held down
bool IsKeyDown(int id)
{
	short kstate = GetKeyState(id);
	// key state will be negative if it's down
	return kstate < 0;
}