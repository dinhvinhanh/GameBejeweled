#include "game.h"
#include <iostream>
#define DELAY 500
#define IMAGE_RES 64 

Game::Game()
{
	SCREEN_WIDTH = 600;
	SCREEN_HEIGHT = 600;
	moves = 0;  
	score = 0;
}

SDL_Texture* Game::loadTexture(string path)
{
	SDL_Texture* new_texture = NULL;

	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if (load_surface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		new_texture = SDL_CreateTextureFromSurface(gScreen, load_surface);
		SDL_FreeSurface(load_surface);
	}
	return new_texture;
}

bool Game::init()
{
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Sekiro", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gScreen = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);;
		}
	}

	// Inti SDL_tt (TEXT Extenstion)
	if (TTF_Init() < 0)
	{
		cout << "Probelm Initing SDL_ttf" << endl;
		success = false;
	}

	// Load Sound
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}

	//Initialize SDL_mixer (Sound Extension)
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	return success;
}

bool Game::loadMedia()
	bool success = true;

	// Load the 5 Shapes
	shapes[GRID_TYPE_DIAMOND] = loadTexture("images/Diamond.png");
	shapes[GRID_TYPE_DODE] = loadTexture("images/Dode.png");
	shapes[GRID_TYPE_HEXAGON] = loadTexture("images/Hexagon.png");
	shapes[GRID_TYPE_SQUARE] = loadTexture("images/Square.png");
	shapes[GRID_TYPE_TRIANGLE] = loadTexture("images/Triangle.png");
	if (shapes[0] == NULL || shapes[1] == NULL || shapes[2] == NULL || shapes[3] == NULL || shapes[4] == NULL)
	{
		printf("Failed to load Basic Shapes!\n");
		success = false;
	}

	menuTexture = loadTexture("images/menu.png");
	if (menuTexture == NULL)
	{
		cout << "Failed to load Menu Image" << endl;
		success = false;
	}

	endgameTexture = loadTexture("images/end_game.png");
	if (endgameTexture == NULL)
	{
		cout << "Failed to load End Game Image" << endl;
		success = false;
	}

	instructionTexture = loadTexture("images/instruction.png");
	if (instructionTexture == NULL)
	{
		cout << "Failed to load Instruction Image" << endl;
		success = false;
	}

	backgroundTexture = loadTexture("images/background.png");
	if (backgroundTexture == NULL)
	{
		cout << "Failed to load Background Image" << endl;
		success = false;
	}

	// Load Celebration Image
	point_shape = loadTexture("images/point.png");
	if (point_shape == NULL)
	{
		cout << "Failed to load Point Image" << endl;
		success = false;
	}

	selection = loadTexture("images/selection.png");
	if (selection == NULL)
	{
		cout << "Failed to load Selection Image" << endl;
		success = false;
	}

	// Load Font (Lazy)
	font = TTF_OpenFont("./images/Lazy.ttf", 20);
	if (font == NULL)
	{
		cout << "Failed To Load Font" << endl;
		success = false;
	}

	// Load Sound
	gMusic = Mix_LoadWAV("./images/high.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;
}

void Game::close()
{
	//Deallocate surfaces
	map<int, SDL_Texture*>::iterator i;
	for (i = shapes.begin(); i != shapes.end(); ++i)
	{
		SDL_DestroyTexture(shapes[i->first]);
		shapes[i->first] = NULL;
	}
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(point_shape);

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	Mix_FreeChunk(gMusic);

	SDL_Quit();
	Mix_Quit();
	IMG_Quit();
}

void Game::animate_point(vector<int>& pos, Grid grid)
{
	gCurrentSurface = point_shape;
	SDL_Rect tmp;
	tmp.w = IMAGE_RES;
	tmp.h = IMAGE_RES;

	tmp.y = pos[0] * IMAGE_RES + UP;
	tmp.x = pos[3] * IMAGE_RES + LEFT;
	SDL_RenderCopy(gScreen, gCurrentSurface, NULL, &tmp);

	tmp.y = pos[1] * IMAGE_RES + UP;
	tmp.x = pos[4] * IMAGE_RES + LEFT;
	SDL_RenderCopy(gScreen, gCurrentSurface, NULL, &tmp);

	tmp.y = pos[2] * IMAGE_RES + UP; 
	tmp.x = pos[5] * IMAGE_RES + LEFT; 
	SDL_RenderCopy(gScreen, gCurrentSurface, NULL, &tmp);

	if (pos[6] != -1)
	{
		tmp.y = pos[6] * IMAGE_RES + UP;
		tmp.x = pos[7] * IMAGE_RES + LEFT;
		SDL_RenderCopy(gScreen, gCurrentSurface, NULL, &tmp);
	}

	if (pos[8] != -1)
	{
		tmp.y = pos[8] * IMAGE_RES + UP;
		tmp.x = pos[9] * IMAGE_RES + LEFT;
		SDL_RenderCopy(gScreen, gCurrentSurface, NULL, &tmp);
	}

	ready_text("WOW", 2);

	SDL_RenderPresent(gScreen);
	SDL_Delay(DELAY);

	animate_grid(grid);
}

void Game::animate_grid(Grid grid)
{
	SDL_RenderClear(gScreen);

	SDL_RenderCopy(gScreen, backgroundTexture, NULL, NULL);

	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			gCurrentSurface = shapes[grid.get_shape(i, j)];
			SDL_Rect tmp;
			tmp.x = j * IMAGE_RES + LEFT; 
			tmp.y = i * IMAGE_RES + UP;
			tmp.w = IMAGE_RES;
			tmp.h = IMAGE_RES;
			SDL_RenderCopy(gScreen, gCurrentSurface, NULL, &tmp);
		}
	}

	//SDL_RenderPresent(gScreen);
}

void Game::ready_text(string text, int type)
{
	SDL_Rect tmp = { 0, SCREEN_HEIGHT - 50, 600, 50 };
	SDL_Color foregroundColor = { 50, 255, 255 };
	SDL_Color backgroundColor = { 0xff,0xff,0xff };
	
	text = text + "   " + "Score: " + to_string(score) + "      " + "Moves: " + to_string(moves) + "   ";
	SDL_Surface* textSurface = TTF_RenderText_Shaded(font, text.c_str(), foregroundColor, backgroundColor);
	textTexture = SDL_CreateTextureFromSurface(gScreen, textSurface);
	if (textTexture == NULL)
	{
		cout << "Problem Creating Text Surface" << endl;
	}
	
	SDL_RenderCopy(gScreen, textTexture, NULL, &tmp);
}

void Game::showMenu()
{
	SDL_RenderClear(gScreen);

	SDL_RenderCopy(gScreen, menuTexture, NULL, NULL);

	SDL_RenderPresent(gScreen);
} 

void Game::showInstruction()
{
	SDL_RenderClear(gScreen);

	SDL_RenderCopy(gScreen, instructionTexture, NULL, NULL);

	SDL_RenderPresent(gScreen);
}

void Game::showBackground()
{
	SDL_RenderClear(gScreen);

	SDL_RenderCopy(gScreen, backgroundTexture, NULL, NULL);

	SDL_RenderPresent(gScreen);
}

void Game::game_main()
{
	Grid grid;
	int x0, y0, x1, y1;
	bool clicked = false;
	SDL_Event e;
	bool display_instructions = true;
	int menu = 1;

	Button button;

	if (!init())
	{
		printf("Failed to initialize!\n");
		return;
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
			return;
		}
		else
		{
			bool close_game = false;


			while (!close_game)
			{
				bool quit = false;

				showMenu();
				while (menu)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							close();
							return;
						}
						menu = button.handlePLayButton(&e);
						if (menu == -1)
						{
							close();
							return;
						}
					}
				}

				showInstruction();
				while (display_instructions)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							close();
							return;
						}
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							display_instructions = false;
						}
					}
				}

				//showBackground();
				grid.load_grid();
				ready_text("READY", 2);
				while (!quit && moves <= 30)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							close();
							return;
						}
						else if (e.key.keysym.sym == SDLK_ESCAPE)
						{
							quit = true;
						}
						else if (e.button.button == SDL_BUTTON_RIGHT)
						{
							clicked = false;
						}
						else if (e.type == SDL_MOUSEBUTTONDOWN)
						{

							if (clicked == false)
							{
								SDL_GetMouseState(&y0, &x0);
								x0 = (x0 - LEFT) / IMAGE_RES;
								y0 = (y0 - UP) / IMAGE_RES;

								clicked = true;
							}
							else
							{
								SDL_GetMouseState(&y1, &x1);
								x1 = (x1 - LEFT) / IMAGE_RES;
								y1 = (y1 - UP) / IMAGE_RES;
								bool isvalid = grid.valid_move(x0, y0, x1, y1);
								int cascade = 0;
								if (isvalid)
								{
									moves++;
									vector<int>tmp(10, -1);    
									while (grid.sequence(tmp))
									{
										Mix_PlayChannel(-1, gMusic, 0); 
										animate_grid(grid);              
										SDL_Delay(DELAY / 10);
										//ready_text("WOW", 2);          
										animate_point(tmp, grid);          
										grid.update_grid(tmp);          
										SDL_Delay(DELAY / 10);
										cascade++;
									}
									score += (cascade == 1) ? 10 : 20 * cascade;   
									//ready_text("READY", 2);
								}
								if (cascade == 0)
								{
									animate_grid(grid);
									ready_text("", 2);
									SDL_RenderPresent(gScreen);
									SDL_Delay(DELAY / 2);
									grid.valid_move(x0, y0, x1, y1);
								}
								clicked = false;
							}
						}
					}
					animate_grid(grid);
					ready_text("", 2);
					SDL_RenderPresent(gScreen);
				}

				SDL_RenderClear(gScreen);
				SDL_RenderCopy(gScreen, endgameTexture, NULL, NULL);
				SDL_RenderPresent(gScreen);

				bool endgame = true;

				while (endgame)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							close();
							return;
						}
						endgame = button.handlePlayAgainButton(&e);
					}
				}
			}
		}
	}

	close();
	return;
}
