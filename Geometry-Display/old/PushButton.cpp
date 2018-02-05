#include "PushButton.hpp"

namespace GUI {
	PushButton::PushButton(sf::RenderWindow & window, std::shared_ptr<sf::Font> font) :
		Button(window, font)
	{
	}

	void PushButton::update(sf::Time & dt) {
		if (!m_bounce) {
			if (sf::Mouse::isButtonPressed(m_mouse_button)) {
				wykobi::point2d<int> mouse_pos = wykobi::make_point(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
				if (pointInside(mouse_pos)) {
					m_active = true;
					m_function();

					setInnerColor(m_active_color);

					m_bounce = true;
				}
			}
		}
		else {
			if (!sf::Mouse::isButtonPressed(m_mouse_button)) {
				m_active = false;
				m_bounce = false;
			}
		}
	}

	void PushButton::setFunction(std::function<void()> func) {
		m_function = func;
	}
}
