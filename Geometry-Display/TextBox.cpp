#include "TextBox.hpp"

namespace GUI {
	TextBox::TextBox(sf::RenderWindow & window, std::shared_ptr<sf::Font> font) :
		UIObject::UIObject(window),
		m_font(font)
	{
		m_sf_text.setFont(*m_font);
		
	}


}