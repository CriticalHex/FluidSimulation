#pragma once
#include<vector>
#include"globals.h"

class Pixel {
public:
	Pixel(sf::Vector2f position);
	~Pixel() {};
	static int index(int x, int y);
	void update(std::vector<Pixel*>& pixels, float dt, sf::Image& image);
	void applyGravity(float dt);
	void addDensity(float dens, float dt);
	void diffuse(std::vector<Pixel*>& pixels, int b, float dt);
	sf::Color color;
	sf::Vector2f c_pos;
	sf::Vector2f l_pos;
	sf::Vector2f c_velocity;
	sf::Vector2f l_velocity;
	float density = 0;
	float diffusion = 0;
	float viscosity = 0.000001;
};