#pragma once
#include "Coord.h"
#include "Vector2.h"
#include "GameTexture.h"
#include <iterator>
#include <list>
#include <math.h>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

class GameObject {
protected:
	Coord pos;
	Coord center;
	float orientation;
	float orientation_degrees;
	Vector2 direction;
	Vector2 size;
	string name;
	SDL_Renderer* renderer;
	list<GameTexture> textures;
public:
	GameObject(Coord pos, Coord center, Vector2 direction, Vector2 size) :
		pos(pos), center(center), direction(direction), size(size) {
		calculate_orientation();
	}

	virtual void draw();
	Coord get_position();
	Coord get_center();
	Vector2 get_direction();
	Vector2 get_size();
	string get_name();
	void update_position();
	void calculate_orientation();
	void set_renderer(SDL_Renderer* value);
	void add_texture(GameTexture texture);
	void set_name(string name);
	void set_direction(Vector2 direction);
private:
	const int ASSET_ROTATION_OFFSET = 90;
};