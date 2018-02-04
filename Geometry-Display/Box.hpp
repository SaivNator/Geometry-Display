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
		Resize
		*/
		virtual void resize(wykobi::vector2d<int> vec);

		/*
		Set size
		*/
		virtual void setSize(wykobi::vector2d<int> vec);

		/*
		Set inner fill color
		*/
		virtual void setInnerColor(sf::Color color);

		/*
		Set outer line color
		*/
		virtual void setOuterColor(sf::Color color);

		/*
		Move
		*/
		virtual void move(wykobi::vector2d<int> vec);

		/*
		Set position
		*/
		virtual void setPosition(wykobi::point2d<int> p);

		/*
		Draw
		*/
		virtual void draw();

	protected:
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

