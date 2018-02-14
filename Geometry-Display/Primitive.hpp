//Author: Sivert Andresen Cubedo
#pragma once

#ifndef Primitive_HEADER
#define Primitive_HEADER

#include <SFML\Graphics.hpp>

#include "GUIBase.hpp"

namespace GUI {
	namespace Primitive {
		class Rect : public sf::Drawable {
		public:
			/*
			Constructor
			*/
			Rect(sf::IntRect rect, sf::Color color);

			/*
			Set size
			*/
			void setSize(sf::Vector2i size);

			/*
			Set pos
			*/
			void setPos(sf::Vector2i pos);

			/*
			Set rect
			*/
			void setRect(sf::IntRect rect);

			/*
			Set color
			*/
			void setColor(sf::Color color);

			/*
			Get size
			*/
			sf::Vector2i getSize();

			/*
			Get pos
			*/
			sf::Vector2i getPos();

			/*
			Get rect
			*/
			sf::IntRect getRect();

			/*
			Get color
			*/
			sf::Color getColor();
		protected:
			/*
			Update draw
			*/
			virtual void updateDraw() = 0;

			//virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
			
			sf::IntRect m_rect;
			sf::Color m_color;
		};

		class FillRect : public Rect {
		public:
			/*
			Constructor
			*/
			FillRect(sf::IntRect rect = sf::IntRect(), sf::Color color = sf::Color());
		protected:
			sf::VertexArray m_vertex;

			virtual void updateDraw() override;
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};

		class LineRect : public Rect {
		public:
			/*
			Constructor
			*/
			LineRect(sf::IntRect rect = sf::IntRect(), sf::Color color = sf::Color());
		protected:
			sf::VertexArray m_vertex;

			virtual void updateDraw() override;
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};
	}
}

#endif // !Primitive_HEADER

