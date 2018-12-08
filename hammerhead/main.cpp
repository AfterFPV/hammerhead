#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <iterator>
#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

#define SPACE_SIZE 320

using namespace std;


struct Vector2
{
	float x;
	float y;

	Vector2(float x, float y) : x(x), y(y) {
	}
};

class Coord {
protected:
	float x, y;
public:
	Coord(float x, float y) : x(x), y(y) {
	}
	int get_intX();
	int get_intY();
	float get_floatX();
	float get_floatY();
};

int Coord::get_intX() {
	return (int)x;
}

int Coord::get_intY() {
	return (int)y;
}

float Coord::get_floatX() {
	return x;
}

float Coord::get_floatY() {
	return y;
}



class AstrObject {
protected:
	Coord pos;
	Coord center;
	float radius;
	Vector2 direction;
	float size;
	int dmg, range;

public:
	AstrObject(Coord pos, Coord center, float radius, Vector2 direction, float size, int dmg, int range) :
		pos(pos), center(center), radius(radius), direction(direction), size(size), dmg(dmg), range(range) {
	}

	Coord get_position();
	Coord get_center();
	Vector2 get_direction();
	float get_radius();
	int get_size();
	void update_position();
private:
	int attack();
};

Coord AstrObject::get_position() {
	return pos;
}

void AstrObject::update_position() {
	pos = Coord(pos.get_floatX() + direction.x, pos.get_floatY() + direction.y);
	center = Coord(center.get_floatX() + direction.x, center.get_floatY() + direction.y);
}


Vector2 AstrObject::get_direction() {

	return direction;
}

Coord AstrObject::get_center() {

	return center;
}

float AstrObject::get_radius() {

	return radius;
}

int AstrObject::get_size() {

	return size;
}

/*
class Ship: public AstrObject {

protected:
int upgrades;
public:
Ship(int x, int y, int dmg, int speed, int range, int size):
AstrObject(x, y, dmg, speed, range, size) {
}
};

class Planet: public AstrObject {

protected:
int upgrades;
public:
Planet(int x, int y, int dmg, int speed, int range, int size):
AstrObject(x, y, dmg, speed, range, size) {
}
};
*/



class Space {
	char two_d[SPACE_SIZE][SPACE_SIZE];
	list<AstrObject> objs;
	SDL_Renderer* renderer;
	int window_width;
	int window_height;
	int tile_width;
	int tile_height;
public:
	void tick();
	void add_object(AstrObject obj);
	void draw_map();
	void print_list();
	void set_renderer(SDL_Renderer* value) {
		this->renderer = value;
	}
	void set_size(int width, int height) {
		this->window_width = width;
		this->window_height = height;

		this->tile_width = width / SPACE_SIZE;
		this->tile_height = height / SPACE_SIZE;
	}
	Space() {
		blank_map();
	}
private:
	void blank_map();
	void update_map();
	void update_orbit(AstrObject &obj);
	void add_center(AstrObject);
};

void Space::tick() {
	update_map();

	cout << "tick" << endl;
}

void Space::add_object(AstrObject obj) {
	objs.push_back(obj);
}

void Space::draw_map() {
	int posX = 0;
	int posY = 0;

	for (int i = 0; i < SPACE_SIZE; i++) {
		posY = i * this->tile_height;

		for (int j = 0; j < SPACE_SIZE; j++) {			
			posX = j * this->tile_width;
			
			char tile_type = two_d[i][j];

			switch (tile_type) {
			case 'x':
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				break;
			case '-':
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				break;
			case '.':
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				break;
			}
			SDL_Rect tile;
			tile.h = tile_height;
			tile.w = tile_width;
			tile.x = posX;
			tile.y = posY;

			SDL_RenderFillRect(renderer, &tile);
			//cout << two_d[i][j] << " ";
		}
		//cout << endl;
	}	

	// Render the changes above
	SDL_RenderPresent(renderer);
}

void Space::print_list() {

	list<AstrObject>::iterator it;
	int x, y;

	for (it = objs.begin(); it != objs.end(); it++) {

		//pos = &(it->get_position());
		x = (int)(it->get_position().get_floatX() + it->get_direction().x);
		y = (int)(it->get_position().get_floatY() + it->get_direction().y);

		cout << "x = " << x << ", y = " << y << endl;
	}
}

void Space::blank_map() {

	for (int i = 0; i < SPACE_SIZE; i++) {
		for (int j = 0; j < SPACE_SIZE; j++) {
			two_d[i][j] = '.';
		}
	}
}

void Space::update_map() {
	blank_map();

	int x, y, size;

	list<AstrObject>::iterator it;

	for (it = objs.begin(); it != objs.end(); it++) {
		it->update_position();
		x = (int)(it->get_position().get_floatX() + it->get_direction().x);
		y = (int)(it->get_position().get_floatY() + it->get_direction().y);

		size = it->get_size();

		two_d[x][y] = 'x';

		for (int i = x; i < x + size; i++) {
			for (int j = y; j < y + size; j++) {
				two_d[i][j] = 'x';
			}
		}

		update_orbit((*it));
	}
}

void Space::update_orbit(AstrObject &obj) {

	float radius;
	const float PI = 3.14159;
	Coord center = obj.get_center();
	int x, y;
	float cent_x, cent_y;
	float tmp_x, tmp_y;

	cent_x = center.get_floatX();
	cent_y = center.get_floatY();
	radius = obj.get_radius();

	for (float angle = 0; angle <= 2 * PI; angle += 0.01) {

		tmp_x = cent_x + (radius * cos(angle));
		tmp_y = cent_y + (radius * sin(angle));

		x = (int)tmp_x;
		y = (int)tmp_y;

		two_d[x][y] = '-';
	}

}

void Space::add_center(AstrObject obj) {


}

Space space;


/*
int main(int argc, char **argv)
{
	cout << "hello" << endl;

	Space one;

	Coord tmp_pos(1, 1);
	Coord tmp_cent(15.5, 15.5);

	Coord earth_pos(14, 14);

	AstrObject intrepid(tmp_pos, tmp_cent, 10.0, 1.0, 1.0, 0, 0);
	//AstrObject bad_guys(10,10,100,43,50,1);
	AstrObject earth(earth_pos, earth_pos, 10.0, 1.0, 4.0, 0, 0);
	//AstrObject moon(20,10,0,0,0,2);

	//one.add_object(intrepid);
	//one.add_object(bad_guys);
	one.add_object(earth);
	//one.add_object(moon);



	one.print_map();
	one.tick();
	one.print_list();
	one.print_map();

	return 0;
}
*/


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

	
	space.set_size(sizeX, sizeY);
	space.set_renderer(renderer);

	return 0;
}



void updateState() {
	space.tick();
}

void draw() {
	space.draw_map();
}

void runGameLoop() {
	int loopCount = 600;

	for (int i = 0; i < loopCount; i++) {
		updateState();
		draw();

		//Pause 1000 ms
		SDL_Delay(100);
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
}


