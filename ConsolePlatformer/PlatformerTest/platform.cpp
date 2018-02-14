#include "game.h"
#include "platform.h"

Platform::Platform(float x, float y, float w) : x(x), y(y), w(w) {}

void Platform::draw()
{
	globalGame->setCursorPos((int)this->x, (int)this->y);
	// just a line of ====================
	for (int dx = 0; dx < this->w; dx++)
		std::cout << "=";
}
