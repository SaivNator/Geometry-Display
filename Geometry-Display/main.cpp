//Author: Sivert Andresen Cubedo

#include <iostream>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>

#include "Window.hpp"

int main() {
	std::string title = "Window";
	Window window(sf::VideoMode(1000, 600), title, sf::Style::Default, sf::ContextSettings(sf::ContextSettings::Default));
	window.join();

	return EXIT_SUCCESS;
}