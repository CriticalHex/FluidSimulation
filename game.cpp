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

void Game::boundLastVelocity() {
	for (int y = 1; y < windowSize.y - 1; y++) { //left
		pixels[Pixel::index(0, y)]->boundLastVelocity(pixels, 3);
	}
	for (int y = 1; y < windowSize.y - 1; y++) { //right
		pixels[Pixel::index(windowSize.x - 1, y)]->boundLastVelocity(pixels, 1);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //top
		pixels[Pixel::index(x, 0)]->boundLastVelocity(pixels, 0);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //bottom
		pixels[Pixel::index(x, windowSize.y - 1)]->boundLastVelocity(pixels, 2);
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

void Game::boundCurrentVelocity() {
	for (int y = 1; y < windowSize.y - 1; y++) { //left
		pixels[Pixel::index(0, y)]->boundCurrentVelocity(pixels, 3);
	}
	for (int y = 1; y < windowSize.y - 1; y++) { //right
		pixels[Pixel::index(windowSize.x - 1, y)]->boundCurrentVelocity(pixels, 1);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //top
		pixels[Pixel::index(x, 0)]->boundCurrentVelocity(pixels, 0);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //bottom
		pixels[Pixel::index(x, windowSize.y - 1)]->boundCurrentVelocity(pixels, 2);
	}

	sf::Vector2f v1 = pixels[Pixel::index(1, 0)]->c_velocity;
	sf::Vector2f v2 = pixels[Pixel::index(0, 1)]->c_velocity;
	pixels[Pixel::index(0, 0)]->c_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(1, windowSize.y - 1)]->c_velocity;
	v2 = pixels[Pixel::index(0, windowSize.y - 2)]->c_velocity;
	pixels[Pixel::index(0, windowSize.y - 1)]->c_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(windowSize.x - 2, 0)]->c_velocity;
	v2 = pixels[Pixel::index(windowSize.x - 1, 1)]->c_velocity;
	pixels[Pixel::index(windowSize.x - 1, 0)]->c_velocity = .5f * (v1 + v2);

	v1 = pixels[Pixel::index(windowSize.x - 2, windowSize.y - 1)]->c_velocity;
	v2 = pixels[Pixel::index(windowSize.x - 1, windowSize.y - 2)]->c_velocity;
	pixels[Pixel::index(windowSize.x - 1, windowSize.y - 1)]->c_velocity = .5f * (v1 + v2);
}

void Game::diffuseVelocity() {
	for (int iteration = 0; iteration < solveIterations; iteration++) {
		for (int y = 1; y < windowSize.y - 1; y++) {
			for (int x = 1; x < windowSize.x - 1; x++) {
				pixels[Pixel::index(x, y)]->diffuseVelocity(pixels, dt.asSeconds());
			}
		}
		boundLastVelocity();
	}
}

void Game::advectVelocity() {
	float dx = dt.asSeconds() * (windowSize.x - 2);
	float dy = dt.asSeconds() * (windowSize.y - 2);
	for (int i = 1; i < windowSize.y - 1; i++) {
		for (int j = 1; j < windowSize.x - 1; j++) {
			pixels[Pixel::index(j, i)]->advectVelocity(pixels, dt.asSeconds(), dx, dy);
		}
	}
	boundCurrentVelocity();
}

void Game::projectVelocity() {
	for (int i = 1; i < windowSize.y - 1; i++) {
		for (int j = 1; j < windowSize.x - 1; j++) {
			pixels[Pixel::index(j, i)]->projectVelocity(pixels, dt.asSeconds());
		}
	}
	boundCurrentVelocity();
}

void Game::diffuseDensity() {
	for (int iteration = 0; iteration < solveIterations; iteration++) {
		for (int y = 1; y < windowSize.y - 1; y++) {
			for (int x = 1; x < windowSize.x - 1; x++) {
				pixels[Pixel::index(x, y)]->diffuseDensity(pixels, dt.asSeconds());
			}
		}
		boundLastDensity();
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
	boundCurrentDensity();
}

void Game::boundCurrentDensity() {
	for (int y = 1; y < windowSize.y - 1; y++) { //left
		pixels[Pixel::index(0, y)]->boundCurrentDensity(pixels, 3);
	}
	for (int y = 1; y < windowSize.y - 1; y++) { //right
		pixels[Pixel::index(windowSize.x - 1, y)]->boundCurrentDensity(pixels, 1);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //top
		pixels[Pixel::index(x, 0)]->boundCurrentDensity(pixels, 0);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //bottom
		pixels[Pixel::index(x, windowSize.y - 1)]->boundCurrentDensity(pixels, 2);
	}

	float d1 = pixels[Pixel::index(1, 0)]->c_density;
	float d2 = pixels[Pixel::index(0, 1)]->c_density;
	pixels[Pixel::index(0, 0)]->c_density = .5f * (d1 + d2);

	d1 = pixels[Pixel::index(1, windowSize.y - 1)]->c_density;
	d2 = pixels[Pixel::index(0, windowSize.y - 2)]->c_density;
	pixels[Pixel::index(0, windowSize.y - 1)]->c_density = .5f * (d1 + d2);

	d1 = pixels[Pixel::index(windowSize.x - 2, 0)]->c_density;
	d2 = pixels[Pixel::index(windowSize.x - 1, 1)]->c_density;
	pixels[Pixel::index(windowSize.x - 1, 0)]->c_density = .5f * (d1 + d2);

	d1 = pixels[Pixel::index(windowSize.x - 2, windowSize.y - 1)]->c_density;
	d2 = pixels[Pixel::index(windowSize.x - 1, windowSize.y - 2)]->c_density;
	pixels[Pixel::index(windowSize.x - 1, windowSize.y - 1)]->c_density = .5f * (d1 + d2);
}

void Game::boundLastDensity() {
	for (int y = 1; y < windowSize.y - 1; y++) { //left
		pixels[Pixel::index(0, y)]->boundLastDensity(pixels, 3);
	}
	for (int y = 1; y < windowSize.y - 1; y++) { //right
		pixels[Pixel::index(windowSize.x - 1, y)]->boundLastDensity(pixels, 1);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //top
		pixels[Pixel::index(x, 0)]->boundLastDensity(pixels, 0);
	}
	for (int x = 1; x < windowSize.x - 1; x++) { //bottom
		pixels[Pixel::index(x, windowSize.y - 1)]->boundLastDensity(pixels, 2);
	}

	float d1 = pixels[Pixel::index(1, 0)]->l_density;
	float d2 = pixels[Pixel::index(0, 1)]->l_density;
	pixels[Pixel::index(0, 0)]->l_density = .5f * (d1 + d2);

	d1 = pixels[Pixel::index(1, windowSize.y - 1)]->l_density;
	d2 = pixels[Pixel::index(0, windowSize.y - 2)]->l_density;
	pixels[Pixel::index(0, windowSize.y - 1)]->l_density = .5f * (d1 + d2);

	d1 = pixels[Pixel::index(windowSize.x - 2, 0)]->l_density;
	d2 = pixels[Pixel::index(windowSize.x - 1, 1)]->l_density;
	pixels[Pixel::index(windowSize.x - 1, 0)]->l_density = .5f * (d1 + d2);

	d1 = pixels[Pixel::index(windowSize.x - 2, windowSize.y - 1)]->l_density;
	d2 = pixels[Pixel::index(windowSize.x - 1, windowSize.y - 2)]->l_density;
	pixels[Pixel::index(windowSize.x - 1, windowSize.y - 1)]->l_density = .5f * (d1 + d2);
}

void Game::update() {
	for (int x = windowSize.x / 2 - 10; x < windowSize.x / 2 + 10; x++) {
		pixels[Pixel::index(x, 0)]->addVelocity(gravity);
	}
	diffuseVelocity();



	diffuseDensity();
	advectDensity();
	for (auto& pixel : pixels) {
		pixel->update(image);
	}
}

void Game::run() {

	initialize();

	while (window.isOpen()) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			mousePos = sf::Mouse::getPosition();
			for (int y = mousePos.y - 4; y <= mousePos.y + 4 && y > 0 && y < windowSize.y; y++) {
				for (int x = mousePos.x - 4; x <= mousePos.x + 4 && x > 0 && x < windowSize.x; x++) {
					pixels[Pixel::index(x, y)]->addDensity(150);
				}
			}
		}
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			mousePos = sf::Mouse::getPosition();
			sf::Vector2i mouseChange = mousePos - lastMousePos;
			for (int y = mousePos.y - 4; y <= mousePos.y + 4 && y > 0 && y < windowSize.y; y++) {
				for (int x = mousePos.x - 4; x <= mousePos.x + 4 && x > 0 && x < windowSize.x; x++) {
					pixels[Pixel::index(x, y)]->addVelocity(sf::Vector2f(mouseChange.x, mouseChange.y));
				}
			}
		}*/
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