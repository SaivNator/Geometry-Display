#include "Button.hpp"

namespace GUI {

	Button::Button(std::shared_ptr<sf::Font> font, sf::IntRect rect, sf::Mouse::Button mouse_button, Color color, String string) :
		m_font(font),
		m_rect(rect),
		m_mouse_button(mouse_button),
		m_color(color),
		m_string(string),
		m_inner_rect(rect, m_color.m_inner_inactive_color),
		m_outer_rect(rect, m_color.m_outer_inactive_color)
	{
		m_text.setFont(*font);
		updateDraw();
	}

	sf::Text & Button::getText() {
		return m_text;
	}

	void Button::draw(sf::RenderWindow & window, sf::Time & dt) {
		window.draw(m_inner_rect);
		window.draw(m_outer_rect);
		window.draw(m_text);
	}

	void Button::setMouseButton(sf::Mouse::Button button) {
		m_mouse_button = button;
	}

	sf::IntRect & Button::getRect() {
		return m_rect;
	}

	Button::Color & Button::getColor() {
		return m_color;
	}

	Button::String & Button::getString() {
		return m_string;
	}

	void Button::updateDraw() {
		sf::Vector2f p(static_cast<float>(m_rect.left), static_cast<float>(m_rect.top));
		sf::Vector2f s(static_cast<float>(m_rect.width), static_cast<float>(m_rect.height));
		m_inner_rect.setRect(m_rect);
		m_outer_rect.setRect(m_rect);
		sf::Color inner_color;
		sf::Color outer_color;
		sf::Color text_color;
		std::string text_string;
		if (!m_active) {
			inner_color = m_color.m_inner_inactive_color;
			outer_color = m_color.m_outer_inactive_color;
			text_color = m_color.m_text_inactive_color;
			text_string = m_string.m_inactive_string;
		}
		else {
			inner_color = m_color.m_inner_active_color;
			outer_color = m_color.m_outer_active_color;
			text_color = m_color.m_text_active_color;
			text_string = m_string.m_active_string;
		}
		m_inner_rect.setColor(inner_color);
		m_outer_rect.setColor(outer_color);
		m_text.setFillColor(text_color);
		m_text.setString(text_string);
		
		//arrange text inside rect
		
		sf::FloatRect textRect = m_text.getLocalBounds();
		m_text.setOrigin(textRect.left + textRect.width / 2.f,
			textRect.top + textRect.height / 2.f);
		m_text.setPosition(p + (s / 2.f));
	}
}
