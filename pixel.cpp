#include"globals.h"
#include"pixel.h"

Pixel::Pixel(sf::Vector2f position) {
	c_pos = position;
	color = sf::Color::Color(8,6,12);
}

void Pixel::update(std::vector< std::vector<Pixel*>>& pixels, float dt, sf::Image& image) {
	color = sf::Color::Color(6 * dt, 8 * dt, 12 * dt);
	image.setPixel(c_pos.x, c_pos.y, color);
}