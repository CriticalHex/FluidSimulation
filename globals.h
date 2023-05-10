#pragma once
#include<SFML/Graphics.hpp>

const sf::Vector2f windowSize = sf::Vector2f(512, 512);
const int windowArea = windowSize.x * windowSize.y;
const sf::Vector2f gravity(0, -9.8);
const int solveIterations = 1;
