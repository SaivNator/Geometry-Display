//Author: Sivert Andresen Cubedo
#include "UIObject.hpp"

namespace GUI {
	UIObject::UIObject(sf::RenderWindow & window) :
		m_window(window)
	{
	}
	sf::RenderWindow & UIObject::getWindow() {
		return m_window;
	}
}
