#include "Box.hpp"

namespace GUI {
	Box::Box(sf::RenderWindow & window, UIRectangle rect) :
		UIObject(window, rect)
	{
	}

	void Box::draw() {
		UIObject::draw();
		
		
	}
}
