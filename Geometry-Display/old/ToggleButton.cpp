#include "ToggleButton.hpp"

namespace GUI {
	ToggleButton::ToggleButton(sf::RenderWindow & window, std::shared_ptr<sf::Font> font) :
		Button(window, font)
	{
	}

	void ToggleButton::update(sf::Time & dt) {
		if (!m_bounce) {
			if (sf::Mouse::isButtonPressed(m_mouse_button)) {
				wykobi::point2d<int> mouse_pos = wykobi::make_point(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
				if (pointInside(mouse_pos)) {
					m_active = !m_active;
					m_function(m_active);
					m_bounce = true;
				}
			}
		}
		else {
			if (!sf::Mouse::isButtonPressed(m_mouse_button)) {
				m_bounce = false;
			}
		}
	}

	void ToggleButton::setFunction(std::function<void(bool)> func) {
		m_function = func;
	}


}