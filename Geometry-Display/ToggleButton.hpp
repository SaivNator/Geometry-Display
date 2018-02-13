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
		ToggleButton(
			std::shared_ptr<sf::Font> font,
			sf::IntRect rect, std::function<void(bool)> func = [](bool) {} ,
			sf::Mouse::Button mouse_button = sf::Mouse::Left, 
			Color color = Color(),
			String string = String()
		);

		/*
		Update
		*/
		virtual void update(sf::RenderWindow & window, sf::Time & dt) override;

		/*
		Set function
		*/
		void setFunction(std::function<void(bool)> func);
	protected:
		bool m_bounce = false;
		std::function<void(bool)> m_func;
	};
}

#endif // !ToggleButton_HEADER

