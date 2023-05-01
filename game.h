#pragma once
#include<vector>
#include"globals.h"
#include"pixel.h"

class Game
{
public:
	Game();
	void run();
private:
	void initialize();
	void garbageCollect();
	std::vector<std::unique_ptr<Pixel>> gameObjects; // std::make_unique<GameObject>();
	sf::RenderWindow window;
	sf::Event gameEvent;
	sf::Color bgColor = sf::Color(6, 8, 12);
	sf::Clock clock;
	sf::Time delta;
};