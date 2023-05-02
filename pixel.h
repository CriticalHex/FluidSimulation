#pragma once
#include<vector>
#include"globals.h"

class Pixel {
public:
	Pixel(sf::Vector2f position);
	~Pixel() {};
	void update(std::vector< std::vector<Pixel*>>& pixels, float dt, sf::Image& image);
	sf::Color color;
	sf::Vector2f c_pos;
	sf::Vector2f l_pos;
	float s;
	float density;
	float diffusion;
	float viscosity;
};