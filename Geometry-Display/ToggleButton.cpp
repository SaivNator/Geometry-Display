#include "ToggleButton.hpp"

namespace GUI {

	ToggleButton::ToggleButton(std::shared_ptr<sf::Font> font, sf::IntRect rect, sf::Mouse::Button mouse_button, std::function<void(bool)> func) :
		Button(font, rect, mouse_button),
		m_func(func)
	{
	}

	void ToggleButton::update(sf::RenderWindow & window, sf::Time & dt) {
		if (sf::Mouse::isButtonPressed(m_mouse_button) && m_rect.contains(sf::Mouse::getPosition(window))) {
			if (!m_bounce) {
				m_bounce = true;
				m_active = !m_active;
				updateDraw();
				m_func(m_active);
			}
		}
		else {
			if (m_bounce) {
				m_bounce = false;
				updateDraw();
			}
		}
	}

}
