#include "Box.hpp"

namespace GUI {
	Box::Box(sf::RenderWindow & window, UIRectangle rect) :
		UIObject(window),
		m_bounding_rect(rect),
		m_box_inner_vertex_array(sf::Triangles, 6),
		m_box_outer_vertex_array(sf::LineStrip, 5)
	{
		applyVertexArray();
	}

	void Box::setSize(wykobi::vector2d<int> vec) {
		m_bounding_rect.m_size = vec;
		applyVertexArray();
	}

	void Box::setInnerColor(sf::Color color) {
		m_inner_fill_color = color;
		applyVertexArray();
	}

	void Box::setOuterColor(sf::Color color) {
		m_outer_line_color = color;
		applyVertexArray();
	}

	bool Box::pointInside(wykobi::point2d<int>& p) {
		return m_bounding_rect.pointInside(p);
	}

	void Box::setPosition(wykobi::point2d<int> p) {
		m_bounding_rect.m_pos = p;
		applyVertexArray();
	}

	void Box::draw() {
		if (m_is_show) {
			if (m_inner_fill) {
				m_window.draw(m_box_inner_vertex_array);
			}
			if (m_outer_line) {
				m_window.draw(m_box_outer_vertex_array);
			}
		}
	}


	void Box::applyVertexArray() {
		sf::Vector2f s(static_cast<float>(m_bounding_rect.m_size.x), static_cast<float>(m_bounding_rect.m_size.y));
		
		sf::Vector2f p0(static_cast<float>(m_bounding_rect.m_pos.x), static_cast<float>(m_bounding_rect.m_pos.y));
		sf::Vector2f p1(p0.x + s.x, p0.y);
		sf::Vector2f p2(p0 + s);
		sf::Vector2f p3(p0.x, p0.y + s.y);

		m_box_inner_vertex_array[0].position = p0;
		m_box_inner_vertex_array[1].position = p1;
		m_box_inner_vertex_array[2].position = p2;
		m_box_inner_vertex_array[3].position = p0;
		m_box_inner_vertex_array[4].position = p2;
		m_box_inner_vertex_array[5].position = p3;

		m_box_inner_vertex_array[0].color = m_inner_fill_color;
		m_box_inner_vertex_array[1].color = m_inner_fill_color;
		m_box_inner_vertex_array[2].color = m_inner_fill_color;
		m_box_inner_vertex_array[3].color = m_inner_fill_color;
		m_box_inner_vertex_array[4].color = m_inner_fill_color;
		m_box_inner_vertex_array[5].color = m_inner_fill_color;

		m_box_outer_vertex_array[0].position = p0;
		m_box_outer_vertex_array[1].position = p1;
		m_box_outer_vertex_array[2].position = p2;
		m_box_outer_vertex_array[3].position = p3;
		m_box_outer_vertex_array[4].position = p0;

		m_box_outer_vertex_array[0].color = m_outer_line_color;
		m_box_outer_vertex_array[1].color = m_outer_line_color;
		m_box_outer_vertex_array[2].color = m_outer_line_color;
		m_box_outer_vertex_array[3].color = m_outer_line_color;
		m_box_outer_vertex_array[4].color = m_outer_line_color;
	}
}
