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

Vector2 GameObject::get_size() {
	return size;
}

void GameObject::set_direction(Vector2 direction) {
	this->direction = direction;
}

void GameObject::draw() {
	list<GameTexture>::iterator it;

	for (it = textures.begin(); it != textures.end(); it++) {
		SDL_Rect display_rect = { this->pos.get_intX() + it->get_relative_pos().get_intX(), this->pos.get_intY() + it->get_relative_pos().get_intY(), it->get_width(), it->get_height() };
		SDL_RenderCopy(renderer, it->get_texture(), NULL, &display_rect);
	}
}

void GameObject::set_renderer(SDL_Renderer* value) {
	this->renderer = value;
}

void GameObject::add_texture(GameTexture texture) {
	textures.push_back(texture);
}

void GameObject::set_name(string name) {
	this->name = name;
}

string GameObject::get_name() {
	return this->name;
}