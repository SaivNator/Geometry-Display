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
		PushButton(sf::RenderWindow & window, std::shared_ptr<sf::Font> font);

		/*
		Update
		*/
		virtual void update(sf::Time & dt);

		/*
		Set functon
		*/
		virtual void setFunction(std::function<void()> func);
	protected:
		std::function<void()> m_function;
	};
}

#endif // !PushButton_HEADER
