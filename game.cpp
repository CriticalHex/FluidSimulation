#include"globals.h"
#include"game.h"
#include<iostream>

Game::Game() {
	window.create(sf::VideoMode(windowSize.x, windowSize.y), "Fluid", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);
	image.create(windowSize.x, windowSize.y, sf::Color::Red);
	texture.loadFromImage(image);
	fluid.setTexture(texture);
}

void Game::initialize() {
	for (int y = 0; y < windowSize.y; y++) {
		for (int x = 0; x < windowSize.x; x++) {
			pixels.push_back(new Pixel(sf::Vector2f(x, y)));
		}
	}
};

void Game::run() {

	initialize();

	while (window.isOpen()) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			mousePos = sf::Mouse::getPosition();
			//std::cout << Pixel::index(mousePos.x, mousePos.y) << std::endl;
			pixels[Pixel::index(mousePos.x, mousePos.y)]->addDensity(50, dt.asSeconds());
		}
		while (window.pollEvent(gameEvent))
		{
			// Close window: exit-----------------
			if (gameEvent.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				window.close();
			}
		}
		//UPDATE-------------------------------------------------------------
		dt = clock.getElapsedTime();
		clock.restart();
		for (auto& pixel : pixels) {
			pixel->update(pixels, dt.asMilliseconds(), image);
		}

		//RENDER--------------------------------------------------------------
		window.clear(bgColor);

		texture.loadFromImage(image);
		window.draw(fluid);

		window.display();
	}
}