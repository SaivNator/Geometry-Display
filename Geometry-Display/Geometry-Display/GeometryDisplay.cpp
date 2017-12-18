//Author: Sivert Andresen Cubedo

#include "GeometryDisplay.hpp"


void GeometryDisplay::Window::create() {
	//std::cout << ">>>" << std::this_thread::get_id() << "<<<: " << "init()\n";
	window_thread = std::thread(&Window::windowHandler, this);
}

void GeometryDisplay::Window::windowHandler() {

	window.create(sf::VideoMode(500, 500), "Default title", sf::Style::Default);



	while (running) {
		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				running = false;
				break;
			default:
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	//kill window
	window.close();
}

void GeometryDisplay::Window::close() {
	running = false;
	join();
}

void GeometryDisplay::Window::join() {
	if (window_thread.joinable()) {
		window_thread.join();
	}
}

//end