#pragma once
#include <stdint.h>

class Color {
public:
	uint8_t r, g, b;
	Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
	}
};
