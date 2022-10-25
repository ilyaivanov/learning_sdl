#include <stdio.h>
#include <SDL.h>
#include "app.h"

// don't do this, this is just an example
SDL_Renderer *renderer;
SDL_Window *window;
int isRunning;
int fullscreen;
void handleEvents();

// static int resizingEventWatcher(void *data, SDL_Event *event)
//{
//	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
//	{
//		SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
//		if (window == (SDL_Window *)data)
//		{
//			printf("reiszing...");
//			render();
//		}
//	}
// }

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTime = 0;
int main()
{

	fullscreen = 0;
	int flags = SDL_WINDOW_RESIZABLE;
	if (fullscreen)
	{
		flags = flags | SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

		window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, flags);
		// SDL_AddEventWatch(resizingEventWatcher, window);
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

	NOW = SDL_GetPerformanceCounter();
	init();
	while (isRunning)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		handleEvents();
		update(deltaTime);
		render(renderer);
	}

	teardown();
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

	if (event.type == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		int w = 0;
		int h = 0;
		SDL_GetRendererOutputSize(renderer, &w, &h);
		printf("%dx%d", w, h);
	}
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = 0;
		break;
	default:
		break;
	}
}
