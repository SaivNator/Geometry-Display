//TEST MAIN

#include <iostream>
#include <string>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>

#include "FileDialog.hpp"

int main() {

	sf::RenderWindow window;

	window.create(sf::VideoMode(500, 500), "TEST", sf::Style::Resize | sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close);

	wykobi::point2d<float> p1 = wykobi::make_point(100.f, 200.f);

	std::cout << "Hello World\n";
	std::cout << p1.x << "\t" << p1.y << "\n";

	FileDialog::OpenDir dialog;

	dialog.create();

	std::cout << dialog.getPath() << "\n";



	return EXIT_SUCCESS;
}


//end