#include "GameObject.h"


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

void GameObject::draw() {

}