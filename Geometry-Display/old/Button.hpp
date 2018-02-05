//Author: Sivert Andrsen Cubedo
#pragma once

#ifndef Button_HEADER
#define Button_HEADER

#include <functional>

#include "TextBox.hpp"

namespace GUI {
	class Button : public TextBox {
	public:
		/*
		Constructor
		*/
		Button(sf::RenderWindow & window, std::shared_ptr<sf::Font> font);

		/*
		Update
		*/
		virtual void update(sf::Time & dt) = 0;

		/*
		Set default string
		*/
		virtual void setDefaultString(std::string string);

		/*
		Set active string
		*/
		virtual void setActiveString(std::string string);

		/*
		Set default color
		*/
		virtual void setDefaultColor(sf::Color color);

		/*
		Set mouse button
		*/
		virtual void setMouseButton(sf::Mouse::Button button);

		/*
		Set active color
		*/
		virtual void setActiveColor(sf::Color color);
	protected:
		bool m_bounce = false;
		bool m_active = false;
		sf::Color m_default_color;
		std::string m_default_string;
		sf::Color m_active_color;
		std::string m_active_string;
		sf::Mouse::Button m_mouse_button = sf::Mouse::Left;
	};
}

#endif // !Button_HEADER

