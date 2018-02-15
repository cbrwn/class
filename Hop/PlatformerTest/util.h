#pragma once

// misc util functions

struct Rect
{
	int left, top, right, bottom;
};

void PrintCenter(const char s[], int y);
void SetConsoleCursorPos(int x, int y);
bool IsKeyDown(int id);