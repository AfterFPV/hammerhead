#pragma once
class Coord {
protected:
	float x, y;
public:
	Coord(float x, float y) : x(x), y(y) {
	}
	int get_intX();
	int get_intY();
	float get_floatX();
	float get_floatY();
};

int Coord::get_intX() {
	return (int)x;
}

int Coord::get_intY() {
	return (int)y;
}

float Coord::get_floatX() {
	return x;
}

float Coord::get_floatY() {
	return y;
}