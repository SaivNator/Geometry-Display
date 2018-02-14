//Author: Sivert Andresen Cubedo
#pragma once

#ifndef Button_HEADER
#define Button_HEADER

#include <iostream>

#include <memory>
#include <functional>

#include "GUIBase.hpp"
#include "Primitive.hpp"

namespace GUI {
	class Button : public GUIBase {
	public:
		struct Color {
			sf::Color m_inner_inactive_color = sf::Color::Blue;
			sf::Color m_inner_active_color = sf::Color::Green;
			sf::Color m_outer_active_color = sf::Color::Black;
			sf::Color m_outer_inactive_color = sf::Color::Black;
			sf::Color m_text_inactive_color = sf::Color::Black;
			sf::Color m_text_active_color = sf::Color::Black;
		};
		struct String {
			std::string m_inactive_string;
			std::string m_active_string;
			unsigned int m_inactive_char_size = 16;
			unsigned int m_active_char_size = 16;
		};

		/*
		Constructor
		*/
		Button(std::shared_ptr<sf::Font> font, sf::IntRect rect, sf::Mouse::Button mouse_button, Color color, String string);
		
		/*
		Draw
		*/
		virtual void draw(sf::RenderWindow & window, sf::Time & dt) override;
		
		/*
		Set mouse button
		*/
		void setMouseButton(sf::Mouse::Button button);

		/*
		Get text
		*/
		sf::Text & getText();

		/*
		Get Rect
		*/
		sf::IntRect & getRect();

		/*
		Get Color
		*/
		Color & getColor();

		/*
		Get String
		*/
		String & getString();
		
		/*
		Update draw objects
		*/
		virtual void updateDraw();
	protected:
		sf::IntRect m_rect;
		bool m_active = false;
		sf::Mouse::Button m_mouse_button;
		std::shared_ptr<sf::Font> m_font;
		Primitive::FillRect m_inner_rect;
		Primitive::LineRect m_outer_rect;
		sf::Text m_text;
		Color m_color;
		String m_string;
	};
}

#endif // !Button_HEADER
