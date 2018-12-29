#pragma once

#include "Coord.h"
#include "Moon.h"
#include "Planet.h"
#include "Sun.h"

using namespace std;

class CelestialBodyFactory
{
public:
	static unique_ptr<Planet> create_planet(string name, Coord pos, float radius, Asset3d asset) {
		return move(create_planet(name, pos, radius, asset, Color(255, 255, 255)));
	}

	static unique_ptr<Planet> create_planet(string name, Coord pos, float radius, Asset3d asset, Color color) {
		Vector2 direction(0, 0);

		unique_ptr<Planet> planet(new Planet(pos, direction, radius));
		planet->set_name(name);

		float diameter = radius * 2;
		planet->set_model(asset.get_model());

		return move(planet);
	}

	static unique_ptr<Moon> create_moon(string name, Coord pos, float radius, Asset3d asset) {
		return move(create_moon(name, pos, radius, asset, Color(255, 255, 255)));
	}

	static unique_ptr<Moon> create_moon(string name, Coord pos, float radius, Asset3d asset, Color color) {
		Vector2 direction(0, 0);

		unique_ptr<Moon> moon(new Moon(pos, direction, radius));
		moon->set_name(name);

		float diameter = radius * 2;
		moon->set_model(asset.get_model());

		return move(moon);
	}

	static unique_ptr<Sun> create_sun(string name, Coord pos, float radius, Asset3d asset) {
		return move(create_sun(name, pos, radius, asset, Color(255, 255, 255)));
	}

	static unique_ptr<Sun> create_sun(string name, Coord pos, float radius, Asset3d asset, Color color) {
		Vector2 direction(0, 0);

		unique_ptr<Sun> sun(new Sun(pos, direction, radius));
		sun->set_name(name);

		float diameter = radius * 2;
		sun->set_model(asset.get_model());

		return move(sun);
	}


private:
	CelestialBodyFactory() {}
};