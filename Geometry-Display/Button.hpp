//Author: Sivert Andresen Cubedo
#pragma once

#ifndef Button_HEADER
#define Button_HEADER

#include <memory>
#include <functional>

#include "GUIBase.hpp"

namespace GUI {
	class Button : public GUIBase {
	public:
		/*
		Constructor
		*/
		Button(sf::RenderWindow & window, std::shared_ptr<sf::Font> font, sf::IntRect rect, sf::Mouse::Button mouse_button);
		
		/*
		Get text
		*/
		sf::Text & getText();

		/*
		Draw
		*/
		virtual void draw() override;

		/*
		Color vars
		*/
		sf::Color m_inner_inactive_color;
		sf::Color m_inner_active_color;
		sf::Color m_outer_active_color;
		sf::Color m_outer_inactive_color;
		sf::Color m_text_inactive_color;
		sf::Color m_text_active_color;
	protected:
		sf::IntRect m_rect;
		bool m_active = false;
		sf::Mouse::Button m_mouse_button;
		std::shared_ptr<sf::Font> m_font;
		sf::VertexArray m_inner_vertex;
		sf::VertexArray m_outer_vertex;
		sf::Text m_text;

		/*
		Update draw objects
		*/
		virtual void updateDraw();
	};
}

#endif // !Button_HEADER
