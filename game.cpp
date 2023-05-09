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

void Game::boundVelocity() {
	for (int y = 1; y < windowSize.y - 1; y++) { //left
		pixels[Pixel::index(0, y)]->boundVelocity(pixels, 3);
	}
	for (int y = 1; y < windowSize.y - 1; y++) { //right
		pixels[Pixel::index(windowSize.x - 1, y)]->boundVelocity(pixels, 1);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //top
		pixels[Pixel::index(x, 0)]->boundVelocity(pixels, 0);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //bottom
		pixels[Pixel::index(x, windowSize.y - 1)]->boundVelocity(pixels, 2);
	}

	sf::Vector2f v1 = pixels[Pixel::index(1, 0)]->l_velocity;
	sf::Vector2f v2 = pixels[Pixel::index(0, 1)]->l_velocity;
	pixels[Pixel::index(0, 0)]->l_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(1, windowSize.y - 1)]->l_velocity;
	v2 = pixels[Pixel::index(0, windowSize.y - 2)]->l_velocity;
	pixels[Pixel::index(0, windowSize.y - 1)]->l_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(windowSize.x - 2, 0)]->l_velocity;
	v2 = pixels[Pixel::index(windowSize.x - 1, 1)]->l_velocity;
	pixels[Pixel::index(windowSize.x - 1, 0)]->l_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(windowSize.x - 2, windowSize.y - 1)]->l_velocity;
	v2 = pixels[Pixel::index(windowSize.x - 1, windowSize.y - 2)]->l_velocity;
	pixels[Pixel::index(windowSize.x - 1, windowSize.y - 1)]->l_velocity = .5f * (v1 + v2);
}

void Game::diffuseVelocity() {
	for (int iteration = 0; iteration < solveIterations; iteration++) {
		for (int y = 1; y < windowSize.y - 1; y++) {
			for (int x = 1; x < windowSize.x - 1; x++) {
				pixels[Pixel::index(x, y)]->diffuseVelocity(pixels, dt.asSeconds());
			}
		}
		boundVelocity();
	}
}

void Game::diffuseDensity() {
	for (int iteration = 0; iteration < solveIterations; iteration++) {
		for (int y = 1; y < windowSize.y - 1; y++) {
			for (int x = 1; x < windowSize.x - 1; x++) {
				pixels[Pixel::index(x, y)]->diffuseDensity(pixels, dt.asSeconds());
			}
		}
		boundDensity();
	}
}

void Game::advectDensity() {
	float dx = dt.asSeconds() * (windowSize.x - 2);
	float dy = dt.asSeconds() * (windowSize.y - 2);
	for (int i = 1; i < windowSize.y - 1; i++) {
		for (int j = 1; j < windowSize.x - 1; j++) {
			pixels[Pixel::index(j, i)]->advectDensity(pixels, dt.asSeconds(), dx, dy);
		}
	}
	boundDensity();
}

void Game::boundDensity() {
	for (int y = 1; y < windowSize.y - 1; y++) { //left
		pixels[Pixel::index(0, y)]->boundVelocity(pixels, 3);
	}
	for (int y = 1; y < windowSize.y - 1; y++) { //right
		pixels[Pixel::index(windowSize.x - 1, y)]->boundVelocity(pixels, 1);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //top
		pixels[Pixel::index(x, 0)]->boundVelocity(pixels, 0);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //bottom
		pixels[Pixel::index(x, windowSize.y - 1)]->boundVelocity(pixels, 2);
	}

	sf::Vector2f v1 = pixels[Pixel::index(1, 0)]->l_velocity;
	sf::Vector2f v2 = pixels[Pixel::index(0, 1)]->l_velocity;
	pixels[Pixel::index(0, 0)]->l_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(1, windowSize.y - 1)]->l_velocity;
	v2 = pixels[Pixel::index(0, windowSize.y - 2)]->l_velocity;
	pixels[Pixel::index(0, windowSize.y - 1)]->l_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(windowSize.x - 2, 0)]->l_velocity;
	v2 = pixels[Pixel::index(windowSize.x - 1, 1)]->l_velocity;
	pixels[Pixel::index(windowSize.x - 1, 0)]->l_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(windowSize.x - 2, windowSize.y - 1)]->l_velocity;
	v2 = pixels[Pixel::index(windowSize.x - 1, windowSize.y - 2)]->l_velocity;
	pixels[Pixel::index(windowSize.x - 1, windowSize.y - 1)]->l_velocity = .5f * (v1 + v2);
}

void Game::update() {
	diffuseVelocity();



	diffuseDensity();
	for (auto& pixel : pixels) {
		pixel->update(image);
	}
}

void Game::run() {

	initialize();

	while (window.isOpen()) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			mousePos = sf::Mouse::getPosition();
			//std::cout << Pixel::index(mousePos.x, mousePos.y) << std::endl;
			pixels[Pixel::index(mousePos.x, mousePos.y)]->addDensity(50, dt.asSeconds());
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			mousePos = sf::Mouse::getPosition();
			//std::cout << Pixel::index(mousePos.x, mousePos.y) << std::endl;
			sf::Vector2i mouseChange = mousePos - lastMousePos;
			pixels[Pixel::index(mousePos.x, mousePos.y)]->addVelocity(sf::Vector2f(mouseChange.x, mouseChange.y), dt.asSeconds());
		}
		lastMousePos = sf::Mouse::getPosition();
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
		update();

		//RENDER--------------------------------------------------------------
		window.clear(bgColor);

		texture.loadFromImage(image);
		window.draw(fluid);

		window.display();
	}
}