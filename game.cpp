#include"game.h"

Game::Game() {
	window.create(sf::VideoMode(windowSize.x, windowSize.y), "Fluid", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	image.create(windowSize.x, windowSize.y, bgColor);
	texture.loadFromImage(image);
	fluid.setTexture(texture);
}

void Game::initialize() {
	//pixels.push_back(std::make_unique<Pixel>());
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
			pixel->update(pixels, dt.asMilliseconds());
		}

		//RENDER--------------------------------------------------------------
		window.clear(bgColor);

		texture.loadFromImage(image);
		window.draw(fluid);

		window.display();
	}
}