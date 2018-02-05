#include "TextBox.hpp"

namespace GUI {
	TextBox::TextBox(sf::RenderWindow & window, std::shared_ptr<sf::Font> font) :
		Box::Box(window, UIRectangle(0, 0, 10, 10)),
		m_font(font)
	{
		m_sf_text.setFont(*m_font);
	}

	void TextBox::update(sf::Time & dt) {
	}

	void TextBox::setFont(std::shared_ptr<sf::Font> font) {
		m_font = font;
		m_sf_text.setFont(*font);
	}

	void TextBox::setString(std::string str) {
		m_sf_text.setString(str);
		autoSize();
	}

	void TextBox::setCharacterSize(int size) {
		m_sf_text.setCharacterSize(static_cast<unsigned int>(size));
		autoSize();
	}

	void TextBox::setTextColor(sf::Color color) {
		m_sf_text.setFillColor(color);
	}

	void TextBox::autoSize() {
		wykobi::vector2d<int> vec;
		vec.x = static_cast<int>(m_sf_text.getGlobalBounds().width);
		vec.y = static_cast<int>(m_sf_text.getGlobalBounds().height);
		Box::setSize(vec);
	}

	void TextBox::draw() {
		Box::draw();
		if (m_is_show) {
			m_window.draw(m_sf_text);
		}
	}


}