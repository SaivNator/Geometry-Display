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

	loadHandler(window);

	//loop
	sf::Clock delta_clock;
	while (m_running) {
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		sf::Time dt = delta_clock.restart();
		
		eventHandler(window);

		updateHandler(window, dt);

		drawHandler(window);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::this_thread::sleep_for(m_sleep_duration - std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
	}
	//close
	window.close();
}

void Window::loadHandler(sf::RenderWindow & window) {
	std::shared_ptr<sf::Font> arial(new sf::Font());
	if (!arial->loadFromFile("../resources/fonts/arial.ttf")) {
		std::cout << "Font load failed\n";
		m_running = false;
	}

	//push
	m_gui_object_vec.push_back(
		std::unique_ptr<GUI::GUIBase>(
			new GUI::PushButton(arial, sf::IntRect(100, 100, 100, 100), sf::Mouse::Left, []() {std::cout << "Push\n"; })
		));
	

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		m_running = false;
	}
}

void Window::updateHandler(sf::RenderWindow & window, sf::Time & dt) {
	for (auto it = m_gui_object_vec.begin(); it != m_gui_object_vec.end(); ++it) {
		(*it)->update(window, dt);
	}
}

void Window::drawHandler(sf::RenderWindow & window) {
	window.clear();

	for (auto it = m_gui_object_vec.begin(); it != m_gui_object_vec.end(); ++it) {
		(*it)->draw(window);
	}

	window.display();
}
