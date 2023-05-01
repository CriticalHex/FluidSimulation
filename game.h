#pragma once
#include"globals.h"

class Game
{
public:
	Game();
	void run();
private:
	void initialize();
	std::vector<Pixel*> pixels; // std::make_unique<Pixel>();
	sf::RenderWindow window;
	sf::Event gameEvent;
	sf::Color bgColor = sf::Color(6, 8, 12);
	sf::Clock clock;
	sf::Time dt;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite fluid;
};