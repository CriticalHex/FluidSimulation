#pragma once
#include"pixel.h"

class Game
{
public:
	Game();
	void run();
private:
	void initialize();
	void update();
	void diffuseVelocity();
	void boundLastVelocity();
	void boundCurrentVelocity();
	void boundCopyLastVelocity();
	void boundCopyCurrentVelocity();
	void advectVelocity();
	void projectLastVelocity();
	void projectCurrentVelocity();
	void diffuseDensity();
	void boundLastDensity();
	void boundCurrentDensity();
	void advectDensity();
	std::vector<Pixel*> pixels;
	sf::RenderWindow window;
	sf::Event gameEvent;
	sf::Color bgColor = sf::Color(6, 8, 12);
	sf::Clock clock;
	sf::Time dt;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite fluid;
	sf::Vector2i mousePos;
	sf::Vector2i lastMousePos;
};