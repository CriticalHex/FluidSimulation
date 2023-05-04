#include"globals.h"
#include"pixel.h"
#include<math.h>


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
	c_pos = position;
	color = sf::Color::Color(8,6,12);
}

void Pixel::update(std::vector<Pixel*>& pixels, float dt, sf::Image& image) {
	color = hsv((int(density) + 50) % 255, 200, density);
	image.setPixel(c_pos.x, c_pos.y, color);
}

void Pixel::addDensity(float dens, float dt) { density += dens; };

void Pixel::diffuse(std::vector<Pixel*>& pixels, int b, float dt)
{
	int i, j, k;
	float a = dt * diffusion * windowArea; //always zero unless I change diffusion
	for (k = 0; k < 20; k++) {
		for (i = 1; i <= windowSize.x; i++) {
			for (j = 1; j <= windowSize.y; j++) {
				pixels[index(i, j)]->c_pos = (pixels[index(i, j)]->l_pos + a * (pixels[index(i - 1, j)]->c_pos + pixels[index(i + 1, j)]->c_pos +
					pixels[index(i, j - 1)]->c_pos + pixels[index(i, j + 1)]->c_pos)) / (1 + 4 * a);
			}
		}
		//set_bnd(N, b, x);
	}
}
