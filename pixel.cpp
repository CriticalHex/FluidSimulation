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
	if (c_density > 255) c_density = 255;
	else if (c_density >= 1) c_density -= 1;
	//color = hsv((int(c_density) + 50) % 255, 200, c_density); //doesn't quite work properly
	//color = sf::Color::Color(c_density, c_density, c_density); //render density
	float mag = sqrt(pow(c_velocity.x, 2) + pow(c_velocity.y, 2));
	color = sf::Color::Color(mag, mag, mag);
	image.setPixel(pos.x, pos.y, color);
}

void Pixel::addDensity(float dens) { c_density += dens; };

void Pixel::addVelocity(sf::Vector2f vel) { c_velocity += vel; };

void Pixel::boundLastVelocity(std::vector<Pixel*>& pixels, int axis) {
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

void Pixel::boundCurrentVelocity(std::vector<Pixel*>& pixels, int axis) {
	sf::Vector2f neighborVelocity;
	int x, y;
	x = pos.x;
	y = pos.y;
	switch (axis) {
	case 0: //top
		neighborVelocity = pixels[index(x, y + 1)]->c_velocity;
		c_velocity = sf::Vector2f(neighborVelocity.x, -neighborVelocity.y);
		break;
	case 1: //right
		neighborVelocity = pixels[index(x - 1, y)]->c_velocity;
		c_velocity = sf::Vector2f(-neighborVelocity.x, neighborVelocity.y);
		break;
	case 2: //bottom
		neighborVelocity = pixels[index(x, y - 1)]->c_velocity;
		c_velocity = sf::Vector2f(neighborVelocity.x, -neighborVelocity.y);
		break;
	case 3: //left
		neighborVelocity = pixels[index(x + 1, y)]->c_velocity;
		c_velocity = sf::Vector2f(-neighborVelocity.x, neighborVelocity.y);
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

void Pixel::undivergeLastVelocity1(std::vector<Pixel*>& pixels) {
	l_velocity =
		(c_velocity +
			a * (pixels[index(pos.x + 1, pos.y)]->l_velocity +
				pixels[index(pos.x - 1, pos.y)]->l_velocity +
				pixels[index(pos.x, pos.y + 1)]->l_velocity +
				pixels[index(pos.x, pos.y - 1)]->l_velocity)) * cReciprocal;
}

void Pixel::advectVelocity(std::vector<Pixel*>& pixels, float dt, float dx, float dy) {
	float dvx = dx * c_velocity.x;
	float dvy = dy * c_velocity.y;
	float x = pos.x - dvx;
	float y = pos.y - dvy;

	if (x < .5) x = .5;
	else if (x > (windowSize.x - 1.5)) x = windowSize.x - 1.5;
	if (y < .5) y = .5;
	else if (y > windowSize.y - 1.5) y = windowSize.y - 1.5;
	int ix, ix1, iy, iy1;
	float s1, s0, t1, t0;
	ix = int(x); iy = int(y); ix1 = ix + 1; iy1 = iy + 1;
	s1 = x - ix; s0 = 1 - s1; t1 = y - iy; t0 = 1 - t1;

	sf::Vector2f d1 = pixels[index(ix, iy)]->l_velocity;
	sf::Vector2f d2 = pixels[index(ix, iy1)]->l_velocity;
	sf::Vector2f d3 = pixels[index(ix1, iy)]->l_velocity;
	sf::Vector2f d4 = pixels[index(ix1, iy1)]->l_velocity;
	c_velocity = s0 * (t0 * d1 + t1 * d2) + s1 * (t0 * d3 + t1 * d4);
}

void Pixel::boundLastDensity(std::vector<Pixel*>& pixels, int axis) {
	int x, y;
	x = pos.x;
	y = pos.y;
	switch (axis) {
	case 0: //top
		l_density = pixels[index(x, y + 1)]->l_density;
		break;
	case 1: //right
		l_density = pixels[index(x - 1, y)]->l_density;
		break;
	case 2: //bottom
		l_density = pixels[index(x, y - 1)]->l_density;
		break;
	case 3: //left
		l_density = pixels[index(x + 1, y)]->l_density;
		break;
	}
}

void Pixel::boundCurrentDensity(std::vector<Pixel*>& pixels, int axis) {
	int x, y;
	x = pos.x;
	y = pos.y;
	switch (axis) {
	case 0: //top
		c_density = pixels[index(x, y + 1)]->c_density;
		break;
	case 1: //right
		c_density = pixels[index(x - 1, y)]->c_density;
		break;
	case 2: //bottom
		c_density = pixels[index(x, y - 1)]->c_density;
		break;
	case 3: //left
		c_density = pixels[index(x + 1, y)]->c_density;
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
	float s1, s0, t1, t0;
	ix = int(x); iy = int(y); ix1 = ix + 1; iy1 = iy + 1;
	s1 = x - ix; s0 = 1 - s1; t1 = y - iy; t0 = 1 - t1;

	float d1 = pixels[index(ix, iy)]->l_density;
	float d2 = pixels[index(ix, iy1)]->l_density;
	float d3 = pixels[index(ix1, iy)]->l_density;
	float d4 = pixels[index(ix1, iy1)]->l_density;
	c_density = s0 * (t0 * d1 + t1 * d2) + s1 * (t0 * d3 + t1 * d4);
}