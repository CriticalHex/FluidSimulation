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


//row(greek p) is density
//u is the velocity vector field
//du/dt is acceleration
//	could be represented as: partiald u/t + u dot (nabla u)
//	nabla u can be said grad of u
//-grad of p is the pressure gradient, so p is pressure
//mu(greek u) nambla^2 u is the viscosity(aka friction)
//F is the external forces
