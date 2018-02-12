//Author: Sivert Andresen Cubedo
#pragma once

#ifndef ListMenu_HEADER
#define ListMenu_HEADER

#include <memory>

#include "GUIBase.hpp"

namespace GUI {
	class ListMenu : GUIBase {
	public:
		struct Color {
			sf::Color m_element_inactive_color;
			sf::Color m_element_active_color;
			sf::Color m_element_inactive_text_color;
			sf::Color m_element_active_text_color;
		};

		/*
		Constructor
		*/
		ListMenu(std::shared_ptr<sf::Font> font);

		/*
		Update
		*/
		void virtual update(sf::RenderWindow & window, sf::Time & dt) override;
	protected:
		std::shared_ptr<sf::Font> font;
	};
}

#endif // !ListMenu_HEADER

