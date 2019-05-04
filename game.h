#pragma once

#include "Button.h"
#include <map>
#include <string>
#include <SDL.h>        // SDL2
#include <SDL_image.h>
#include <SDL_ttf.h>    // Extension for TEXT Rendering
#include <SDL_mixer.h>  // Extension for Sound Playing

#include "grid.h"       // To Use the Grid Class

using namespace std;

#define LEFT 40
#define UP 20

class Game {
public:
	void game_main();
	Game();
private:
	//Screen dimension constants
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	SDL_Window* gWindow;            //The window we'll be rendering to
	SDL_Renderer* gScreen;    //The surface contained by the window
	SDL_Texture* gCurrentSurface;   //Current displayed image

	TTF_Font* font = NULL;
	SDL_Texture* textTexture = NULL;
	SDL_Texture* menuTexture = NULL;
	SDL_Texture* endgameTexture = NULL;
	SDL_Texture* instructionTexture = NULL;
	SDL_Texture* backgroundTexture = NULL;
	SDL_Texture* selection = NULL;
	Mix_Chunk* gMusic = NULL;

	map<int, SDL_Texture*>shapes;   // Contains the 5 Shapes
	SDL_Texture* point_shape = NULL; // Contains Celebration Image
	int score;
	int moves;

	enum {
		GRID_TYPE_DIAMOND,
		GRID_TYPE_DODE,
		GRID_TYPE_HEXAGON,
		GRID_TYPE_SQUARE,
		GRID_TYPE_TRIANGLE
	};

	bool init();
	SDL_Texture* loadTexture(string path);
	bool loadMedia();
	void animate_point(vector<int>&, Grid);
	void animate_grid(Grid);
	void ready_text(string, int);	// ko su dung
	void showMenu();
	void showInstruction();
	void showBackground();
	void close();
};