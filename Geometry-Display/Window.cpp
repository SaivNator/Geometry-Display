#include "Window.hpp"

Window::Window(sf::VideoMode mode, const std::string & title, sf::Uint32 style, const sf::ContextSettings & settings) :
	m_running(true),
	m_sleep_duration(16),
	m_thread(&Window::windowHandler, this, mode, title, style, settings)
{
}

void Window::join() {
	if (m_thread.joinable()) {
		m_thread.join();
	}
}

void Window::close() {
	m_running = false;
}

void Window::windowHandler(sf::VideoMode mode, const std::string & title, sf::Uint32 style, const sf::ContextSettings & settings) {
	sf::RenderWindow window(mode, title, style, settings);

	//load
	//loop
	sf::Clock delta_clock;
	while (m_running) {
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		sf::Time dt = delta_clock.restart();
		
		eventHandler(window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			m_running = false;
		}

		updateHandler(window, dt);
		drawHandler(window);
		

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::this_thread::sleep_for(m_sleep_duration - std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
	}
	//close
	window.close();
}

void Window::eventHandler(sf::RenderWindow & window) {
	sf::Event e;
	while (window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			m_running = false;
			break;
		default:
			break;
		}
	}
}

void Window::updateHandler(sf::RenderWindow & window, sf::Time & dt) {

}

void Window::drawHandler(sf::RenderWindow & window) {
	window.clear();

	window.display();
}
