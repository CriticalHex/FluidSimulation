#pragma once
#include<vector>

class Pixel {
public:
	Pixel(sf::Vector2f position);
	~Pixel() {};
	void update(std::vector<Pixel*> pixels, float dt);
	sf::Color color;
	sf::Vector2f pos;
	float density = 0;
};