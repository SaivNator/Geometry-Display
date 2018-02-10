//Author: Sivert Andresen Cubedo
#pragma once

#ifndef GUIBase_HEADER
#define GUIBase_HEADER

#include <SFML\Graphics.hpp>

namespace GUI {
	class GUIBase {
	public:
		/*
		Constructor
		*/
		GUIBase(sf::RenderWindow & window);

		/*
		Update
		*/
		virtual void update(sf::Time & dt) = 0;

		/*
		Draw
		*/
		virtual void draw() = 0;

	protected:
		sf::RenderWindow & m_window;
	};
}

#endif // !GUIBase_HEADER
