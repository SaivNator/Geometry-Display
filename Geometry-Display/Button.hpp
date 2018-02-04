//Author: Sivert Andrsen Cubedo
#pragma once

#ifndef Button_HEADER
#define Button_HEADER

#include <functional>

#include "UIObject.hpp"

namespace GUI {
	class Button : public UIObject {
	public:
		struct ButtonSettings
		{
			sf::Color m_default_color = sf::Color::Blue;
			
			sf::Color m_active_color = sf::Color::Red;
		};

	protected:
		
	};
}

#endif // !Button_HEADER

