//Author: Sivert Andresen Cubedo
#pragma once

#ifndef UIRectangle_HEADER
#define UIRectangle_HEADER

#include <wykobi.hpp>

namespace GUI {

	struct UIRectangle {
		wykobi::point2d<int> m_pos;
		wykobi::vector2d<int> m_size;

		/*
		Constructor
		*/
		UIRectangle() : 
			m_pos(wykobi::make_point(0, 0)),
			m_size(wykobi::make_vector(0, 0))
		{
		}
		UIRectangle(wykobi::vector2d<int> size) :
			m_pos(wykobi::make_point(0, 0)),
			m_size(size)
		{
		}
		UIRectangle(wykobi::point2d<int> pos, wykobi::vector2d<int> size) :
			m_pos(pos),
			m_size(size)
		{
		}
		UIRectangle(wykobi::rectangle<int> rect) :
			m_pos(rect[0]),
			m_size(rect[1] - rect[0])
		{
		}
		UIRectangle(int x, int y, int w, int h) :
			m_pos(wykobi::make_point(x, y)),
			m_size(wykobi::make_vector(w, h))
		{
		}

		/*
		Get wykobi rectangle
		*/
		wykobi::rectangle<int> getWykobiRect() {
			return wykobi::make_rectangle(m_pos, m_pos + m_size);
		}

		/*
		Get corner e
		*/
		wykobi::point2d<int> corner(int e) {
			assert(e >= 0 && e < 4);
			return wykobi::rectangle_corner(getWykobiRect(), e);
		}

		/*
		Check if point is inside rectangle
		*/
		bool pointInside(wykobi::point2d<int> & p) {
			return wykobi::point_in_rectangle(p, getWykobiRect());
		}

		//template <typename T>
		//wykobi::rectangle<T> orderRectangle(wykobi::rectangle<T> & rect) {
		//	return wykobi::make_rectangle<T>(
		//		std::min(rect[0].x, rect[1].x),
		//		std::min(rect[0].y, rect[1].y),
		//		std::max(rect[0].x, rect[1].x),
		//		std::max(rect[0].y, rect[1].y)
		//		);
		//}
	};

}

#endif // !UIRectangle_HEADER

