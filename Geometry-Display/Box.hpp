//Author: Sivert Andresen Cubedo
#pragma once

#ifndef Box_HEADER
#define Box_HEADER

#include "UIObject.hpp"

namespace GUI {
	class Box : public UIObject {
	public:
		/*
		Constructor
		*/
		Box(sf::RenderWindow & window, UIRectangle rect);

		/*
		Draw
		*/
		virtual void draw();

	protected:
		bool m_inner_fill = true;
		sf::Color m_inner_fill_color = sf::Color::White;
		bool m_outer_line = true;
		sf::Color m_outer_line_color = sf::Color::Black;
	};
}

#endif // !Box_HEADER

