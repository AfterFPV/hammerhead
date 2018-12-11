#pragma once

#include "Coord.h"

class CelestialBodyFactory
{
public:
	static Planet create_planet(string name, Coord pos, float radius, DisplayAsset asset) {
		Coord center = Coord(pos.get_floatX() + radius, pos.get_floatY() + radius);
		Vector2 direction(0, 0);

		Planet planet(pos, center, direction, radius);
		planet.set_name(name);

		float diameter = radius * 2;
		GameTexture texture(asset.get_texture(), Coord(0, 0), diameter, diameter);
		planet.add_texture(texture);

		return planet;
	}

	static Moon create_moon(string name, Coord pos, float radius, DisplayAsset asset) {
		Coord center = Coord(pos.get_floatX() + radius, pos.get_floatY() + radius);
		Vector2 direction(0, 0);

		Moon moon(pos, center, direction, radius);
		moon.set_name(name);

		float diameter = radius * 2;
		GameTexture texture(asset.get_texture(), Coord(0, 0), diameter, diameter);
		moon.add_texture(texture);

		return moon;
	}


private:
	CelestialBodyFactory() {}
};