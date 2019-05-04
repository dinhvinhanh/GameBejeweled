#include "Button.h"



Button::Button()
{
	button[play] = { 177, 75, 247, 238 };
	button[quit] = { 510, 510, 80, 80 };
	button[play_again] = {200, 247, 200, 109};
}


Button::~Button()
{
	;
}

int Button::handlePLayButton(SDL_Event* e)
{
	if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_Point mouse = { x, y };
		if (SDL_PointInRect(&mouse, &button[play]) == SDL_TRUE)
		{
			return 0;
		}
		else if (SDL_PointInRect(&mouse, &button[quit]) == SDL_TRUE)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	return 1;
}

int Button::handlePlayAgainButton(SDL_Event* e)
{
	if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_Point mouse = { x, y };
		if (SDL_PointInRect(&mouse, &button[play_again]) == SDL_TRUE)
		{
			return 0;
		}
		else if (SDL_PointInRect(&mouse, &button[quit]) == SDL_TRUE)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	return 1;
}
