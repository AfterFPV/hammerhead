#pragma once
class Coord {
protected:
	float x, y;
public:
	Coord(float x, float y) : x(x), y(y) {
	}
	int get_intX() {
		return (int)x;
	}
	int get_intY() {
		return (int)y;
	}
	float get_floatX() {
		return x;
	}
	float get_floatY() {
		return y;
	}
};
