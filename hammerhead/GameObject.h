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

	Coord get_position();
	Coord get_center();
	Vector2 get_direction();
	float get_radius();
	int get_size();
	void update_position();
private:

};

Coord GameObject::get_position() {
	return pos;
}

void GameObject::update_position() {
	pos = Coord(pos.get_floatX() + direction.x, pos.get_floatY() + direction.y);
	center = Coord(center.get_floatX() + direction.x, center.get_floatY() + direction.y);
}


Vector2 GameObject::get_direction() {

	return direction;
}

Coord GameObject::get_center() {

	return center;
}

float GameObject::get_radius() {

	return radius;
}

int GameObject::get_size() {

	return size;
}