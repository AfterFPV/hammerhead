#pragma once

#include "Coord.h"
#include "Ship.h"
#include "Asset3d.h"

using namespace std;

class ShipFactory
{
public:
	static unique_ptr<Ship> create(string name, Coord pos, Vector2 size, Asset3d asset) {
		Vector2 direction(0, 0);
		float damage = 2.0;
		float range = 2.0;

		unique_ptr<Ship> ship(new Ship(pos, direction, size, damage, range));
		ship->set_name(name);

		ship->set_model(asset.get_model());

		return move(ship);
	}

private:
	ShipFactory() {}
};