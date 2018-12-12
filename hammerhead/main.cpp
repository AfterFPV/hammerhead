#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <iterator>
#include <iostream>
#include <chrono>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "CelestialBody.h"
#include "Coord.h"
#include "Moon.h"
#include "Space.h"
#include "Planet.h"
#include "CelestialBodyFactory.h"
#include "Ship.h"
#include "ShipFactory.h"
#include "Vector2.h"

using namespace std;

constexpr int SPACE_SIZE = 32;


Space space(SPACE_SIZE, SPACE_SIZE);
SDL_Event inputEvent;
SDL_Surface* window_surface;
SDL_Renderer* renderer;
DisplayAssets assets;

void initialize_map() {
	Coord sun_pos(320, 320);
	float sun_radius = 50.0;
	unique_ptr<SpaceObject> sun = CelestialBodyFactory::create_sun("Sun", sun_pos, sun_radius, assets.get_asset("planet-large"), Color(251, 184, 41));

	Coord earth_pos(140, 140);
	float earth_radius = 18.0;
	unique_ptr<SpaceObject> earth = CelestialBodyFactory::create_planet("Earth", earth_pos, earth_radius, assets.get_asset("planet-large"), Color(42, 143, 189));
	Orbit* sun_orbit = new Orbit(sun.get(), 2);
	earth->set_orbit(sun_orbit);

	Coord moon_pos(240, 240);
	float moon_radius = 4.5;
	unique_ptr<SpaceObject> moon = CelestialBodyFactory::create_moon("Moon", moon_pos, moon_radius, assets.get_asset("planet"), Color(216,216,216));
	Orbit* earth_orbit = new Orbit(earth.get(), 5);
	moon->set_orbit(earth_orbit);

	Coord ship_pos(300, 300);
	Vector2 ship_direction(1, 1);
	Vector2 ship_size(8, 8);
	unique_ptr<Ship> ship = ShipFactory::create("intrepid", ship_pos, ship_size, assets.get_asset("ship"));
	ship->set_direction(ship_direction);

	space.add_object(move(sun));
	space.add_object(move(earth));
	space.add_object(move(moon));
	space.add_object(move(ship));
}

int setup_renderer() {
	int posX = 100;
	int posY = 200;
	int sizeX = 1024;
	int sizeY = 800;
	SDL_Window* window;


	// Initialize SDL
	// ==========================================================
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		// Something failed, print error and exit.
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	// Create and init the window
	// ==========================================================
	window = SDL_CreateWindow("hammerhead", posX, posY, sizeX, sizeY, 0);

	if (window == nullptr)
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return -1;
	}

	window_surface = SDL_GetWindowSurface(window);


	int imgFlags = IMG_INIT_PNG;

	int rees = IMG_Init(4);
	
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());		
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


	assets.load();
	assets.setup_textures(renderer);

	return 0;
}

void update_state() {
	space.tick();
}

void draw() {
	space.draw_map();
}


void handle_input() {
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

void run_game_loop() {
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
			handle_input();

			if (inputEvent.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		if (duration_cast<chrono::nanoseconds>(clock::now() - accum_start).count() >= 16666666) {
			// do render updates every 60th of a second
			update_state();
			draw();

			accum_start = clock::now();
			loopCount++;
		}

		end = clock::now();
	}
}

int main(int argc, char* args[])
{	
	int setupResult = setup_renderer();

	if (setupResult != 0) {
		return setupResult;
	}

	initialize_map();
	run_game_loop();

	return 0;
}