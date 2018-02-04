//Author: Sivert Andresen Cubedo
#pragma once

#ifndef TextBox_HEADER
#define TextBox_HEADER

#include "Box.hpp"

namespace GUI {
	class TextBox : public Box {
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
		Set text color
		*/
		virtual void setTextColor(sf::Color color);

		/*
		Auto size bounding box according to string and charactersize
		*/
		virtual void autoSize();
		
		/*
		Draw
		*/
		virtual void draw();

	protected:
		std::shared_ptr<sf::Font> m_font;
	private:
		sf::Text m_sf_text;
	};

}

#endif // !TextBox_HEADER


