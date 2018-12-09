#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <iterator>
#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>
#include "AstrObject.h"
#include "Coord.h"
#include "Space.h"
#include "Vector2.h"

using namespace std;

#define SPACE_SIZE 320

Space space(SPACE_SIZE, SPACE_SIZE);
SDL_Event inputEvent;

void initializeMap() {
	Coord earth_pos(140, 140);
	Vector2 earthDirection(0, 0);
	AstrObject earth(earth_pos, earth_pos, 100.0, earthDirection, 8.0, 0, 0);
	space.add_object(earth);

	Coord ship_pos(200, 200);
	Vector2 shipDirection(1, 1);
	AstrObject ship(ship_pos, ship_pos, 10.0, shipDirection, 4.0, 0, 0);
	space.add_object(ship);
}

int setupRenderer() {
	int posX = 100;
	int posY = 200;
	int sizeX = 1024;
	int sizeY = 800;
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Initialize SDL
	// ==========================================================
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		// Something failed, print error and exit.
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return -1;
	}

	// Create and init the window
	// ==========================================================
	window = SDL_CreateWindow("hammerhead", posX, posY, sizeX, sizeY, 0);

	if (window == nullptr)
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return -1;
	}

	// Create and init the renderer
	// ==========================================================
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return -1;
	}

	// Render something
	// ==========================================================

	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);

	// Set color of renderer to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear the window and make it all black
	SDL_RenderClear(renderer);

	// Render the changes above ( which up until now had just happened behind the scenes )
	SDL_RenderPresent(renderer);

	
	space.set_window_size(sizeX, sizeY);
	space.set_renderer(renderer);

	return 0;
}

void updateState() {
	space.tick();
}

void draw() {
	space.draw_map();
}

void handleInput() {
	/*
		if( event.type == SDL_KEYDOWN )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_UP: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel += DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel -= DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel += DOT_WIDTH / 2; break;
		}
	}
	else if( event.type == SDL_KEYUP )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_UP: yVel += DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel += DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel -= DOT_WIDTH / 2; break;
		}
	}

	x += xVel;

	if( ( x < 0 ) || ( x + DOT_WIDTH > SCREEN_WIDTH ) )
	{
		x -= xVel;
	}

	y += yVel;

	if( ( y < 0 ) || ( y + DOT_HEIGHT > SCREEN_HEIGHT ) )
	{
		y -= yVel;
	}
	*/
}

void runGameLoop() {
	bool quit = false;
	int loopMax = 600;
	int loopCount = 0;
	using clock = std::chrono::high_resolution_clock;
	using milliseconds = std::chrono::milliseconds;
	using std::chrono::duration_cast;

	auto start = clock::now(), end = clock::now();
	uint64_t diff = duration_cast<milliseconds>(end - start).count();

	auto accum_start = clock::now();
	while ((quit == false) && (loopCount < loopMax)) {
		start = clock::now();
		diff = duration_cast<milliseconds>(end - start).count();

		//Events
		while (SDL_PollEvent(&inputEvent))
		{
			handleInput();

			if (inputEvent.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		if (duration_cast<chrono::nanoseconds>(clock::now() - accum_start).count() >= 16666666) {
			// do render updates every 60th of a second
			updateState();
			draw();

			accum_start = clock::now();
			loopCount++;
		}

		end = clock::now();
	}
}

int main(int argc, char* args[])
{
	int setupResult = setupRenderer();

	if (setupResult != 0) {
		return setupResult;
	}

	initializeMap();
	runGameLoop();

	return 0;
}