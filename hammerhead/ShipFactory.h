#pragma once

#include "Coord.h"
#include "Ship.h"
#include "DisplayAsset.h"

class ShipFactory
{
public:
	static Ship create(string name, Coord pos, Vector2 size, DisplayAsset asset) {
		Coord center = pos;
		Vector2 direction(0, 0);
		float damage = 2.0;
		float range = 2.0;

		Ship ship(pos, center, direction, size, damage, range);
		ship.set_name(name);

		GameTexture texture(asset.get_texture(), Coord(0, 0), size.x, size.y);
		ship.add_texture(texture);

		return ship;
	}

private:
	ShipFactory() {}
};