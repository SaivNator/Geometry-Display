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
		ToggleButton(sf::RenderWindow & window, std::shared_ptr<sf::Font> font);

		/*
		Update
		*/
		virtual void update(sf::Time & dt);

		/*
		Set functon
		*/
		virtual void setFunction(std::function<void(bool)> func);

	protected:
		std::function<void(bool)> m_function;
	};
}

#endif // !ToggleButton_HEADER

