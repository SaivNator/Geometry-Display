//Author: Sivert Andresen Cubedo
#pragma once

#ifndef ToggleButton_HEADER
#define ToggleButton_HEADER

#include "Button.hpp"

namespace GUI {
	class ToggleButton : public Button {
	public:
		/*
		Constructor
		*/
		ToggleButton(std::shared_ptr<sf::Font> font, sf::IntRect rect, sf::Mouse::Button mouse_button, std::function<void(bool)> func);

		/*
		Update
		*/
		virtual void update(sf::RenderWindow & window, sf::Time & dt) override;
	protected:
		bool m_bounce = false;
		std::function<void(bool)> m_func;
	};
}

#endif // !ToggleButton_HEADER

