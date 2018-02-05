//Author: Sivert Andresen Cubedo
#pragma once

#ifndef UIObject_HEADER
#define UIObject_HEADER

#include <vector>
#include <memory>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>

#include "UIRectangle.hpp"

namespace GUI {
	class UIObject : public sf::Drawable, sf::Transformable {
	public:
		/*
		Constructor
		*/
		UIObject(sf::RenderWindow & window);

		/*
		Update
		*/
		virtual void update(sf::Time & dt) = 0;

		/*
		Draw
		*/
		virtual void draw() = 0;
	
		/*
		Get window
		*/
		sf::RenderWindow & getWindow();
	protected:
		sf::RenderWindow & m_window;
	};

}

#endif // end UIObject_HEADER

