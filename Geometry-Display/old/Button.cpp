#include "Button.hpp"

namespace GUI {
	Button::Button(sf::RenderWindow & window, std::shared_ptr<sf::Font> font) : 
		TextBox(window, font)
	{
	}

	void Button::setDefaultString(std::string string) {
		m_default_string = string;
		if (!m_active) {
			TextBox::setString(m_default_string);
		}
	}

	void Button::setActiveString(std::string string) {
		m_active_string = string;
		if (m_active) {
			TextBox::setString(m_active_string);
		}
	}

	void Button::setDefaultColor(sf::Color color) {
		m_default_color = color;
		if (!m_active) {
			TextBox::setInnerColor(m_default_color);
		}
	}

	void Button::setMouseButton(sf::Mouse::Button button) {
		m_mouse_button = button;
	}

	void Button::setActiveColor(sf::Color color) {
		m_active_color = color;
		if (!m_active) {
			TextBox::setInnerColor(m_active_color);
		}
	}


}
