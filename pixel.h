#pragma once
#include<vector>
#include"globals.h"

class Pixel {
public:
	Pixel(sf::Vector2f position);
	~Pixel() {};
	static int index(int x, int y);
	void update(sf::Image& image);
	void applyGravity(float dt);
	void addDensity(float dens);
	void addVelocity(sf::Vector2f vel);

	void diffuseDensity(std::vector<Pixel*>& pixels, float dt);
	void advectDensity(std::vector<Pixel*>& pixels, float dx, float dy);
	void boundLastDensity(std::vector<Pixel*>& pixels, int axis);
	void boundCurrentDensity(std::vector<Pixel*>& pixels, int axis);

	void linearSolveVelocity(std::vector<Pixel*>& pixels, float a, float c);
	void advectVelocity(std::vector<Pixel*>& pixels, float dx, float dy);
	void dropCurrentXVelocity(std::vector<Pixel*>& pixels);
	void dropLastXVelocity(std::vector<Pixel*>& pixels);
	void lessenLastVelocity(std::vector<Pixel*>& pixels);
	void lessenCurrentVelocity(std::vector<Pixel*>& pixels);
	void boundLastVelocity(std::vector<Pixel*>& pixels, int axis);
	void boundCurrentVelocity(std::vector<Pixel*>& pixels, int axis);
	void boundCopyLastVelocity(std::vector<Pixel*>& pixels, int axis);
	void boundCopyCurrentVelocity(std::vector<Pixel*>& pixels, int axis);

	sf::Color color;
	sf::Vector2f pos;
	sf::Vector2f c_velocity;
	sf::Vector2f l_velocity;
	float c_density = 0;
	float l_density = 0;
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
