#pragma once
#include"pixel.h"

class Game
{
public:
	Game();
	void run();
private:
	void initialize();
	std::vector< std::vector<Pixel*>> pixels;
	sf::RenderWindow window;
	sf::Event gameEvent;
	sf::Color bgColor = sf::Color(6, 8, 12);
	sf::Clock clock;
	sf::Time dt;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite fluid;
};