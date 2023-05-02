#include"globals.h"
#include"game.h"

Game::Game() {
	window.create(sf::VideoMode(windowSize.x, windowSize.y), "Fluid", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	image.create(windowSize.x, windowSize.y, sf::Color::Red);
	texture.loadFromImage(image);
	fluid.setTexture(texture);
}

void Game::initialize() {
	for (int i = 0; i < windowSize.y; i++) {
		for (int j = 0; j < windowSize.x; j++) {
			pixels.push_back(new Pixel(sf::Vector2f(j, i)));
		}
	}
};

void Game::run() {

	initialize();

	while (window.isOpen()) {
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