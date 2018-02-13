#include "Primitive.hpp"

namespace GUI {
	namespace Primitive {
		Rect::Rect(sf::IntRect rect, sf::Color color) :
			m_rect(rect),
			m_color(color)
		{
		}
		void Rect::setSize(sf::Vector2i size) {
			m_rect.width = size.x;
			m_rect.height = size.y;
			updateDraw();
		}
		void Rect::setPos(sf::Vector2i pos) {
			m_rect.left = pos.x;
			m_rect.top = pos.y;
			updateDraw();
		}
		void Rect::setRect(sf::IntRect rect) {
			m_rect = rect;
			updateDraw();
		}
		void Rect::setColor(sf::Color color) {
			m_color = color;
			updateDraw();
		}
		sf::Vector2i Rect::getSize() {
			return sf::Vector2i(m_rect.width, m_rect.height);
		}
		sf::Vector2i Rect::getPos() {
			return sf::Vector2i(m_rect.left, m_rect.top);
		}
		sf::IntRect Rect::getRect() {
			return m_rect;
		}
		sf::Color Rect::getColor() {
			return m_color;
		}
		FillRect::FillRect(sf::IntRect rect, sf::Color color) :
			Rect::Rect(rect, color),
			m_vertex(sf::Triangles, 6)
		{
			updateDraw();
		}
		void FillRect::updateDraw() {
			sf::Vector2f p(static_cast<float>(m_rect.left), static_cast<float>(m_rect.top));
			sf::Vector2f s(static_cast<float>(m_rect.width), static_cast<float>(m_rect.height));

			sf::Vector2f p0(p);
			sf::Vector2f p1(p.x + s.x, p.y);
			sf::Vector2f p2(p + s);
			sf::Vector2f p3(p.x, p.y + s.y);

			m_vertex[0].position = p0;
			m_vertex[1].position = p1;
			m_vertex[2].position = p2;
			m_vertex[3].position = p0;
			m_vertex[4].position = p2;
			m_vertex[5].position = p3;

			m_vertex[0].color = m_color;
			m_vertex[1].color = m_color;
			m_vertex[2].color = m_color;
			m_vertex[3].color = m_color;
			m_vertex[4].color = m_color;
			m_vertex[5].color = m_color;
		}
		void FillRect::draw(sf::RenderTarget & target, sf::RenderStates states) const {
			target.draw(m_vertex);
		}
		LineRect::LineRect(sf::IntRect rect, sf::Color color) :
			Rect(rect, color),
			m_vertex(sf::LineStrip, 5)
		{
			updateDraw();
		}
		void LineRect::updateDraw() {
			sf::Vector2f p(static_cast<float>(m_rect.left), static_cast<float>(m_rect.top));
			sf::Vector2f s(static_cast<float>(m_rect.width), static_cast<float>(m_rect.height));

			sf::Vector2f p0(p);
			sf::Vector2f p1(p.x + s.x, p.y);
			sf::Vector2f p2(p + s);
			sf::Vector2f p3(p.x, p.y + s.y);

			m_vertex[0].position = p0;
			m_vertex[1].position = p1;
			m_vertex[2].position = p2;
			m_vertex[3].position = p3;
			m_vertex[4].position = p0;

			m_vertex[0].color = m_color;
			m_vertex[1].color = m_color;
			m_vertex[2].color = m_color;
			m_vertex[3].color = m_color;
			m_vertex[4].color = m_color;
		}
		void LineRect::draw(sf::RenderTarget & target, sf::RenderStates states) const {
			target.draw(m_vertex);
		}
	}
}
