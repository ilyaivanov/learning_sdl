#include "SDL.h"
#undef main

// don't do this, this is just an example
SDL_Renderer *renderer;
SDL_Window *window;
int isRunning;
int fullscreen;
void handleEvents();
void update();
void render();
void init();

#define SCREEN_WIDTH 1500

// please don't put all your code in main like I did.
int WinMain()
{

	fullscreen = 0;
	int flags = 0;
	flags = SDL_WINDOW_RESIZABLE;
	if (fullscreen)
	{
		flags = flags | SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

		window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 500, flags);
		if (window)
		{
			SDL_SetWindowMinimumSize(window, 100, 100);
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			isRunning = 1;
		}
	}

	init();
	while (isRunning)
	{
		handleEvents();
		update();
		render();
	}

	// frees memory associated with renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window); // error here
	SDL_Quit();

	return 0;
}

// handles any events that SDL noticed.
void handleEvents()
{
	// the only event we'll check is the  SDL_QUIT event.
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = 0;
		break;
	default:
		break;
	}
}
int direction = 0;
SDL_Rect rect;

void init()
{
	rect.x = 0;
	rect.y = 0;
	rect.w = 100;
	rect.h = 20;
}
// simple render function
void render()
{
	SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 121, 0, 121, 255);

	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer); // copy to screen

	// your stuff to render would typically go here.
	SDL_RenderPresent(renderer);
}

// simple update function
void update()
{
	if (direction == 0)
	{
		rect.x += 1;
	}
	else
	{
		rect.x -= 1;
	}

	if (rect.x > 1250 && direction == 0)
	{
		direction = 1;
	}
	else if (rect.x <= 0 && direction == 1)
	{
		direction = 0;
	}
}