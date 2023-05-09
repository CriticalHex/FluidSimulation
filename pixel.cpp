#include"globals.h"
#include"pixel.h"
#include<math.h>
#include<iostream>

using namespace std;

int Pixel::index(int x, int y) {
	return x + windowSize.x * y;
}

sf::Color hsv(int hue, float sat, float val)
{
	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1.f - sat);
	float q = val * (1.f - sat * f);
	float t = val * (1.f - sat * (1 - f));

	switch (h)
	{
	default:
	case 0:
	case 6: return sf::Color(val * 255, t * 255, p * 255);
	case 1: return sf::Color(q * 255, val * 255, p * 255);
	case 2: return sf::Color(p * 255, val * 255, t * 255);
	case 3: return sf::Color(p * 255, q * 255, val * 255);
	case 4: return sf::Color(t * 255, p * 255, val * 255);
	case 5: return sf::Color(val * 255, p * 255, q * 255);
	}
}

Pixel::Pixel(sf::Vector2f position) {
	pos = position;
	color = sf::Color::Color(8,6,12);
}

void Pixel::update(sf::Image& image) {
	//diffuse(pixels, dt);
	color = hsv((int(c_density) + 50) % 255, 200, c_density);
	image.setPixel(pos.x, pos.y, color);
	c_density *= .9;
}

void Pixel::addDensity(float dens, float dt) { c_density += dens; };

void Pixel::addVelocity(sf::Vector2f vel, float dt) { c_velocity += vel; };

//void Pixel::boundVelocity(std::vector<Pixel*>& pixels, int axis) {
//	sf::Vector2f neighborVelocity;
//	sf::Vector2f workingVelocity;
//	for (int i = 1; i < windowSize.y - 1; i++) {
//		neighborVelocity = pixels[index(1, i)]->c_velocity;
//		workingVelocity.x = -neighborVelocity.x;
//		workingVelocity.y = neighborVelocity.y;
//		pixels[index(0, i)]->c_velocity = workingVelocity;
//		neighborVelocity = pixels[index(windowSize.x - 2, i)]->c_velocity;
//		workingVelocity.x = -neighborVelocity.x;
//		workingVelocity.y = neighborVelocity.y;
//		pixels[index(windowSize.x - 1, i)]->c_velocity = workingVelocity;
//	}
//
//	for (int j = 1; j < windowSize.x - 1; j++) {
//		neighborVelocity = pixels[index(j, 1)]->c_velocity;
//		workingVelocity.x = neighborVelocity.x;
//		workingVelocity.y = -neighborVelocity.y;
//		pixels[index(j, 0)]->c_velocity = workingVelocity;
//		neighborVelocity = pixels[index(j, windowSize.y - 2)]->c_velocity;
//		workingVelocity.x = neighborVelocity.x;
//		workingVelocity.y = -neighborVelocity.y;
//		pixels[index(j, windowSize.y - 1)]->c_velocity = workingVelocity;
//	}
//
//	sf::Vector2f v1 = pixels[index(1, 0)]->c_velocity;
//	sf::Vector2f v2 = pixels[index(0, 1)]->c_velocity;
//	pixels[index(0, 0)]->c_velocity = .5f * (v1 + v2);
//
//	v1 = pixels[index(1, windowSize.y - 1)]->c_velocity;
//	v2 = pixels[index(0, windowSize.y - 2)]->c_velocity;
//	pixels[index(0, windowSize.y - 1)]->c_velocity = .5f * (v1 + v2);
//
//	v1 = pixels[index(windowSize.x - 2, 0)]->c_velocity;
//	v2 = pixels[index(windowSize.x - 1, 1)]->c_velocity;
//	pixels[index(windowSize.x - 1, 0)]->c_velocity = .5f * (v1 + v2);
//
//	v1 = pixels[index(windowSize.x - 2, windowSize.y - 1)]->c_velocity;
//	v2 = pixels[index(windowSize.x - 1, windowSize.y - 2)]->c_velocity;
//	pixels[index(windowSize.x - 1, windowSize.y - 1)]->c_velocity = .5f * (v1 + v2);
//
//}

void Pixel::boundVelocity(std::vector<Pixel*>& pixels, int axis) {
	sf::Vector2f neighborVelocity;
	int x, y;
	x = pos.x;
	y = pos.y;
	switch (axis) {
	case 0: //top
		neighborVelocity = pixels[index(x, y + 1)]->l_velocity;
		l_velocity = sf::Vector2f(neighborVelocity.x, -neighborVelocity.y);
		break;
	case 1: //right
		neighborVelocity = pixels[index(x - 1, y)]->l_velocity;
		l_velocity = sf::Vector2f(-neighborVelocity.x, neighborVelocity.y);
		break;
	case 2: //bottom
		neighborVelocity = pixels[index(x, y - 1)]->l_velocity;
		l_velocity = sf::Vector2f(neighborVelocity.x, -neighborVelocity.y);
		break;
	case 3: //left
		neighborVelocity = pixels[index(x + 1, y)]->l_velocity;
		l_velocity = sf::Vector2f(-neighborVelocity.x, neighborVelocity.y);
		break;
	}
}

void Pixel::diffuseVelocity(std::vector<Pixel*>& pixels, float dt) {
	float a = dt * diffusion * (windowSize.x - 2) * (windowSize.y - 2);
	float c = 1 + 6 * a; //a,c = 0,1 pretty sure
	float cReciprocal = 1 / c; //would also be 1
	l_velocity =
		(c_velocity +
			a * (pixels[index(pos.x + 1, pos.y)]->l_velocity +
				pixels[index(pos.x - 1, pos.y)]->l_velocity +
				pixels[index(pos.x, pos.y + 1)]->l_velocity +
				pixels[index(pos.x, pos.y - 1)]->l_velocity)) * cReciprocal;
}

void Pixel::boundDensity(std::vector<Pixel*>& pixels, int axis) {
	sf::Vector2f neighborVelocity;
	int x, y;
	x = pos.x;
	y = pos.y;
	switch (axis) {
	case 0: //top
		neighborVelocity = pixels[index(x, y + 1)]->l_velocity;
		l_velocity = sf::Vector2f(neighborVelocity.x, -neighborVelocity.y);
		break;
	case 1: //right
		neighborVelocity = pixels[index(x - 1, y)]->l_velocity;
		l_velocity = sf::Vector2f(-neighborVelocity.x, neighborVelocity.y);
		break;
	case 2: //bottom
		neighborVelocity = pixels[index(x, y - 1)]->l_velocity;
		l_velocity = sf::Vector2f(neighborVelocity.x, -neighborVelocity.y);
		break;
	case 3: //left
		neighborVelocity = pixels[index(x + 1, y)]->l_velocity;
		l_velocity = sf::Vector2f(-neighborVelocity.x, neighborVelocity.y);
		break;
	}
}

void Pixel::diffuseDensity(std::vector<Pixel*>& pixels, float dt) {
	float a = dt * diffusion * (windowSize.x - 2) * (windowSize.y - 2);
	float c = 1 + 6 * a; //a,c = 0,1 pretty sure
	float cReciprocal = 1 / c; //would also be 1
	l_density =
		(c_density +
			a * (pixels[index(pos.x + 1, pos.y)]->l_density +
				pixels[index(pos.x - 1, pos.y)]->l_density +
				pixels[index(pos.x, pos.y + 1)]->l_density +
				pixels[index(pos.x, pos.y - 1)]->l_density)) * cReciprocal;
}

void Pixel::advectDensity(std::vector<Pixel*>& pixels, float dt, float dx, float dy) {
	float dvx = dx * c_velocity.x;
	float dvy = dy * c_velocity.y;
	float x = pos.x - dvx;
	float y = pos.y - dvy;

	if (x < .5) x = .5;
	else if (x > (windowSize.x - 1.5)) x = windowSize.x - 1.5;
	if (y < .5) y = .5;
	else if (y > windowSize.y - 1.5) y = windowSize.y - 1.5;
	int ix, ix1, iy, iy1;
	float s1, s0, d1, d0;
	//https://editor.p5js.org/codingtrain/sketches/9kVfB4BF2
}