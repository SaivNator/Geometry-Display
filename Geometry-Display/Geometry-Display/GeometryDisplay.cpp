//Author: Sivert Andresen Cubedo

#include "GeometryDisplay.hpp"


void GeometryDisplay::Window::init() {
	//std::cout << ">>>" << std::this_thread::get_id() << "<<<: " << "init()\n";
	window.create(sf::VideoMode(500, 500), "Default title", sf::Style::Default);

	window_thread = std::thread(&Window::inputLoopFunc, this);

}

void GeometryDisplay::Window::inputLoopFunc() {
	
	sf::Event e;
	while (window.waitEvent(e)) {

		//std::cout << ">>>" << std::this_thread::get_id() << "<<<: " << "event\n";

		//handle event
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}	
	}
}

void GeometryDisplay::Window::close() {

	//auto id = std::this_thread::get_id();
	//std::cout << "ok" << " " << "close\n";

	//window.close();
}

		//std::unique_lock<std::mutex> lock(window_thread_mutex);
		//window_thread_cv.wait(lock, [&] { return window_thread_update_waiting; });
		//lock.unlock();


//end