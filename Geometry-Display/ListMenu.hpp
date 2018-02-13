//Author: Sivert Andresen Cubedo
#pragma once

#ifndef ListMenu_HEADER
#define ListMenu_HEADER

#include <memory>
#include <functional>

#include "GUIBase.hpp"

namespace GUI {
	class ListMenu : public GUIBase {
	public:
		struct Color {
			sf::Color m_element_inactive_color;
			sf::Color m_element_active_color;
			sf::Color m_element_inactive_text_color;
			sf::Color m_element_active_text_color;
		};

		struct Element {
			/*
			Constructor
			*/
			Element() :
				m_title("<insert text>"),
				m_func([]() {}),
				m_inner_vertex(sf::Triangles, 6),
				m_outer_vertex(sf::LineStrip, 5)
			{
			}			

			/*
			Update draw
			*/
			void updateDraw(sf::IntRect rect);


			std::string m_title;
			std::function<void()> m_func;

			sf::VertexArray m_inner_vertex;
			sf::VertexArray m_outer_vertex;
			sf::Text m_text;

		};

		/*
		Constructor
		*/
		ListMenu(std::shared_ptr<sf::Font> font, sf::Vector2i position);

		/*
		Update
		*/
		void virtual update(sf::RenderWindow & window, sf::Time & dt) override;

		/*
		Draw
		*/
		void virtual draw(sf::RenderWindow & window, sf::Time & dt) override;

		/*
		Update draw objects
		*/
		virtual void updateDraw();
	protected:
		std::shared_ptr<sf::Font> m_font;
		sf::Vector2i m_position;
		sf::Vector2i m_element_size;
		Color m_color;
		std::vector<Element> m_element_vec;
	};
}

#endif // !ListMenu_HEADER

