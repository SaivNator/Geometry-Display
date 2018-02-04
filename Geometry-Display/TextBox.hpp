//Author: Sivert Andresen Cubedo
#pragma once

#ifndef TextBox_HEADER
#define TextBox_HEADER

#include "UIObject.hpp"

namespace GUI {
	class TextBox : public UIObject {
	public:
		/*
		Constructor
		*/
		TextBox(sf::RenderWindow & window, std::shared_ptr<sf::Font> font);

		/*
		Set font
		*/
		virtual void setFont(std::shared_ptr<sf::Font> font);

		/*
		Set string
		*/
		virtual void setString(std::string str);

		/*
		Set char size
		*/
		virtual void setCharacterSize(int size);

		/*
		Auto size bounding box accoring to string and charactersize
		*/
		virtual void autoSize();

		/*
		Draw
		*/
		virtual void draw();

	protected:
		sf::Text m_sf_text;
		sf::Color m_color;
		std::string m_string;
		std::shared_ptr<sf::Font> m_font;
	};

}

#endif // !TextBox_HEADER


