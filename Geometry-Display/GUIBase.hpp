//Author: Sivert Andresen Cubedo
#pragma once

#ifndef GUIBase_HEADER
#define GUIBase_HEADER

#include <SFML\Graphics.hpp>

namespace GUI {
	class GUIBase {
	public:
		/*
		Update
		*/
		virtual void update(sf::RenderWindow & window, sf::Time & dt) = 0;

		/*
		Draw
		*/
		virtual void draw(sf::RenderWindow & window) = 0;

	protected:
		
	};
}

#endif // !GUIBase_HEADER
