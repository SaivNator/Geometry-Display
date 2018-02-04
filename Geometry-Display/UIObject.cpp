//Author: Sivert Andresen Cubedo
#include "UIObject.hpp"

namespace GUI {

	UIObject::UIObject(sf::RenderWindow & window) :
		m_window(window)
	{
	}

	UIObject::UIObject(sf::RenderWindow & window, UIRectangle rect) :
		m_window(window),
		m_bounding_rect(rect)
	{
	}

	void UIObject::update(sf::Time & dt) {
		if (!m_children.empty()) {
			for (auto & child : m_children) {
				child.update(dt);
			}
		}
	}

	void UIObject::setShow(bool v) {
		m_isShow = v;
	}

	void UIObject::setResizeable(bool v) {
		m_isResizeable = v;
	}

	void UIObject::setMoveable(bool v) {
		m_isMoveable = v;
	}

	void UIObject::resize(wykobi::vector2d<int> vec) {
		m_bounding_rect.m_size = m_bounding_rect.m_size - vec;
	}

	void UIObject::setSize(wykobi::vector2d<int> vec) {
		m_bounding_rect.m_size = vec;
	}

	void UIObject::move(wykobi::vector2d<int> vec) {
		m_bounding_rect.m_pos = m_bounding_rect.m_pos + vec;
	}

	void UIObject::setPosition(wykobi::point2d<int> p) {
		m_bounding_rect.m_pos = p;
	}

	bool UIObject::pointInside(wykobi::point2d<int>& p) {
		return m_bounding_rect.pointInside(p);
	}

	void UIObject::draw() {
		if (!m_children.empty()) {
			for (auto & child : m_children) {
				child.draw();
			}
		}
	}

}
