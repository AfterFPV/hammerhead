#pragma once
#include "Coord.h"
#include "Vector2.h"

class GameObject {
protected:
	Coord pos;
	Coord center;
	float radius;
	Vector2 direction;
	float size;
	int dmg, range;

public:
	GameObject(Coord pos, Coord center, float radius, Vector2 direction, float size, int dmg, int range) :
		pos(pos), center(center), radius(radius), direction(direction), size(size), dmg(dmg), range(range) {
	}

	virtual void draw();
	Coord get_position();
	Coord get_center();
	Vector2 get_direction();
	float get_radius();
	int get_size();
	void update_position();

private:

};