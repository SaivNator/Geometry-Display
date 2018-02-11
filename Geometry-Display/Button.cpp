#include "Button.hpp"

namespace GUI {

	Button::Button(std::shared_ptr<sf::Font> font, sf::IntRect rect, sf::Mouse::Button mouse_button) :
		m_font(font),
		m_rect(rect),
		m_mouse_button(mouse_button),
		m_inner_vertex(sf::Triangles, 6),
		m_outer_vertex(sf::LineStrip, 5)
	{
		updateDraw();
	}

	sf::Text & Button::getText() {
		return m_text;
	}

	void Button::draw(sf::RenderWindow & window) {
		window.draw(m_inner_vertex);
		window.draw(m_outer_vertex);
		window.draw(m_text);
	}

	void Button::updateDraw() {
		sf::Vector2f p(static_cast<float>(m_rect.left), static_cast<float>(m_rect.top));
		sf::Vector2f s(static_cast<float>(m_rect.width), static_cast<float>(m_rect.height));

		sf::Vector2f p0(p);
		sf::Vector2f p1(p.x + s.x, p.y);
		sf::Vector2f p2(p + s);
		sf::Vector2f p3(p.x, p.y + s.y);

		m_inner_vertex[0].position = p0;
		m_inner_vertex[1].position = p1;
		m_inner_vertex[2].position = p2;
		m_inner_vertex[3].position = p0;
		m_inner_vertex[4].position = p2;
		m_inner_vertex[5].position = p3;

		m_outer_vertex[0].position = p0;
		m_outer_vertex[1].position = p1;
		m_outer_vertex[2].position = p2;
		m_outer_vertex[3].position = p3;
		m_outer_vertex[4].position = p0;

		m_text.setPosition(s);

		sf::Color inner_color;
		sf::Color outer_color;
		sf::Color text_color;
		if (!m_active) {
			inner_color = m_color.m_inner_inactive_color;
			outer_color = m_color.m_outer_inactive_color;
			text_color = m_color.m_text_inactive_color;
		}
		else {
			inner_color = m_color.m_inner_active_color;
			outer_color = m_color.m_outer_active_color;
			text_color = m_color.m_text_active_color;
		}

		m_inner_vertex[0].color = inner_color;
		m_inner_vertex[1].color = inner_color;
		m_inner_vertex[2].color = inner_color;
		m_inner_vertex[3].color = inner_color;
		m_inner_vertex[4].color = inner_color;
		m_inner_vertex[5].color = inner_color;

		m_outer_vertex[0].color = outer_color;
		m_outer_vertex[1].color = outer_color;
		m_outer_vertex[2].color = outer_color;
		m_outer_vertex[3].color = outer_color;
		m_outer_vertex[4].color = outer_color;

		m_text.setFillColor(text_color);
	}
}
