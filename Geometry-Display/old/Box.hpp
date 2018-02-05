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
		Update
		*/
		virtual void update(sf::Time & dt) = 0;

		/*
		Set size
		*/
		virtual void setSize(wykobi::vector2d<int> vec);

		/*
		Set position
		*/
		virtual void setPosition(wykobi::point2d<int> p);

		/*
		Set inner fill color
		*/
		virtual void setInnerColor(sf::Color color);

		/*
		Set outer line color
		*/
		virtual void setOuterColor(sf::Color color);

		/*
		Check if point is inside box
		*/
		virtual bool pointInside(wykobi::point2d<int> & p);

		/*
		Draw
		*/
		virtual void draw();

	protected:
		UIRectangle m_bounding_rect;
		bool m_is_show = true;
		bool m_is_resizeable = false;
		bool m_is_moveable = false;
		bool m_inner_fill = true;
		sf::Color m_inner_fill_color = sf::Color::White;
		bool m_outer_line = true;
		sf::Color m_outer_line_color = sf::Color::Black;
	private:
		sf::VertexArray m_box_inner_vertex_array;
		sf::VertexArray m_box_outer_vertex_array;
		void applyVertexArray();
	};
}

#endif // !Box_HEADER

