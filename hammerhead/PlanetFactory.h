#pragma once

#include "Coord.h"
#include "Planet.h"

class PlanetFactory
{
public:
	static Planet create(string name, Coord pos, float radius, DisplayAsset asset) {
		Coord center = pos;
		Vector2 direction(0, 0);

		Planet planet(pos, center, direction, radius);
		planet.set_name(name);

		GameTexture texture(asset.get_texture(), Coord(0, 0), radius, radius);
		planet.add_texture(texture);

		return planet;
	}

private:
	PlanetFactory() {}
};