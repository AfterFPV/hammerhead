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

	calculate_orientation();
}

void GameObject::draw() {
	list<GameTexture>::iterator it;

	for (it = textures.begin(); it != textures.end(); it++) {
		SDL_Rect display_rect = { this->pos.get_intX() + it->get_relative_pos().get_intX(), this->pos.get_intY() + it->get_relative_pos().get_intY(), it->get_width(), it->get_height() };
		SDL_SetTextureColorMod(it->get_texture(), it->get_color().r, it->get_color().g, it->get_color().b);
		
		float angle = this->orientation_degrees + ASSET_ROTATION_OFFSET;
		SDL_RenderCopyEx(renderer, it->get_texture(), NULL, &display_rect, angle, NULL, SDL_FLIP_NONE);
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

void GameObject::calculate_orientation() {
	this->orientation = atan2(0.5, 0.5);
	this->orientation_degrees = this->orientation * 180 / M_PI;
}