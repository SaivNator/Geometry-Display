#include "PushButton.hpp"

#include <iostream>

namespace GUI {

	PushButton::PushButton(std::shared_ptr<sf::Font> font, sf::IntRect rect, std::function<void()> func, sf::Mouse::Button mouse_button, Color color, String string) :
		Button(font, rect, mouse_button, color, string),
		m_func(func)
	{
	}

	void PushButton::update(sf::RenderWindow & window, sf::Time & dt) {
		if (sf::Mouse::isButtonPressed(m_mouse_button) && m_rect.contains(sf::Mouse::getPosition(window))) {
			if (!m_bounce) {
				m_active = true;
				m_bounce = true;
				updateDraw();
				m_func();
			}
		}
		else {
			if (m_bounce) {
				m_active = false;
				m_bounce = false;
				updateDraw();
			}
		}
	}

	void PushButton::setFunction(std::function<void()> func) {
		m_func = func;
	}

}
