#pragma once

#include <SDL.h>

enum {
	play, quit, play_again
};

class Button
{
public:
	Button();
	~Button();

	int handlePLayButton(SDL_Event* e);

	int handlePlayAgainButton(SDL_Event* e);
	// int handleModeButton(SDL_Event* e);

private:
	SDL_Rect button[3];
};

