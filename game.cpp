#include"game.h"

Game::Game() {
	window.create(sf::VideoMode(1920, 1080), "Fluid", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
}


void GameObject::draw(sf::RenderWindow& window) {
	window.draw(rect);
};

void Game::initialize() {
	gameObjects.push_back(std::make_unique<Pixel>());
};

void Game::garbageCollect() {
	std::vector<std::vector<std::unique_ptr<GameObject>>::iterator> toDelete;
	for (std::vector<std::unique_ptr<GameObject>>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {
		if ((*iter).get()->remove) {
			(*iter).release();
			toDelete.push_back(iter);
		}
	}
	for (auto& iter : toDelete) {
		gameObjects.erase(iter);
	}
}

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
		delta = clock.getElapsedTime();
		clock.restart();
		for (std::unique_ptr<GameObject>& object : gameObjects) {
			object.get()->update(gameObjects, delta.asMilliseconds());
		}

		//RENDER--------------------------------------------------------------
		window.clear(bgColor);

		for (std::unique_ptr<GameObject>& object : gameObjects) {
			object.get()->draw(window);
		}

		window.display();
		garbageCollect();
	}
}