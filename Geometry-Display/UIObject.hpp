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
		UIObject(sf::RenderWindow & window, UIRectangle rect);

		/*
		Update
		*/
		virtual void update(sf::Time & dt);

		/*
		Set show
		*/
		virtual void setShow(bool v);

		/*
		Set resizeable
		*/
		virtual void setResizeable(bool v);

		/*
		Set moveable
		*/
		virtual void setMoveable(bool v);

		/*
		Resize
		*/
		virtual void resize(wykobi::vector2d<int> vec);

		/*
		Set size
		*/
		virtual void setSize(wykobi::vector2d<int> vec);

		/*
		Move
		*/
		virtual void move(wykobi::vector2d<int> vec);

		/*
		Set position
		*/
		virtual void setPosition(wykobi::point2d<int> p);

		/*
		Check if point is inside UIObject
		*/
		virtual bool pointInside(wykobi::point2d<int> & p);

		/*
		Draw
		*/
		virtual void draw();
	
	protected:
		sf::RenderWindow & m_window;
		std::vector<UIObject> m_children;
		UIRectangle m_bounding_rect;
		bool m_is_show = true;
		bool m_is_resizeable = false;
		bool m_is_moveable = false;
	};

}

#endif // end UIObject_HEADER

