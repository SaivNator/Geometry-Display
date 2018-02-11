//Author: Sivert Andresen Cubedo
#pragma once

#ifndef PushButton_HEADER
#define PushButton_HEADER

#include "Button.hpp"

namespace GUI {
	class PushButton : public Button {
	public:
		/*
		Constructor
		*/
		PushButton(std::shared_ptr<sf::Font> font, sf::IntRect rect, sf::Mouse::Button mouse_button, std::function<void()> func);

		/*
		Update
		*/
		virtual void update(sf::RenderWindow & window, sf::Time & dt) override;
	protected:
		bool m_bounce = false;
		std::function<void()> m_func;
	};
}

#endif // !PushButton_HEADER

