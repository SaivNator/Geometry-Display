//Author: Sivert Andresen Cubedo

#include "GeometryDisplay.hpp"

using namespace GeometryDisplay;

ToggleButton::ToggleButton(sf::IntRect button_area) {
	area = button_area;
}

void ToggleButton::click(const sf::Vector2i mouse_pos) {
	if (!bounce) {
		if (area.contains(mouse_pos)) {
			toggle = !toggle;
			bounce = true;
		}
	}
}

void ToggleButton::release() {
	bounce = false;
}

bool ToggleButton::getState() {
	return toggle;
}

void ToggleButton::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	sf::VertexArray vertex_array(sf::Triangles);
	sf::Text text;
	wykobi::rectangle<float> rect = wykobi::make_rectangle((float)area.left, (float)area.top, (float)area.left + (float)area.width, (float)area.top + (float)area.height);
	std::vector<wykobi::triangle<float, 2>> triangle_vec;
	wykobi::algorithm::polygon_triangulate<wykobi::point2d<float>>(wykobi::make_polygon(rect), std::back_inserter(triangle_vec));
	for (wykobi::triangle<float, 2> & tri : triangle_vec) {
		for (std::size_t i = 0; i < tri.size(); ++i) {
			sf::Vertex v;
			v.position.x = tri[i].x;
			v.position.y = tri[i].y;
			if (toggle) {
				v.color = toggle_color;
			}
			else {
				v.color = not_toggle_color;
			}
			vertex_array.append(v);
		}
	}
	text.setPosition((float)area.left, (float)area.top);
	text.setCharacterSize(text_char_size);
	if (toggle) {
		text.setString(toggle_text);
		text.setFillColor(toggle_text_color);
	}
	else {
		text.setString(not_toggle_text);
		text.setFillColor(not_toggle_text_color);
	}
	target.draw(vertex_array, states);
	target.draw(text, states);
}

Window::Window() { 
	text_font = std::shared_ptr<sf::Font>(new sf::Font());
	if (!text_font->loadFromFile("fonts/arial.ttf")) {
		std::cout << "Font load failed\n";
	}
}

Window::Window(std::shared_ptr<sf::Font> font_ptr) {
	text_font = font_ptr;
}

void Window::create() {
	window_thread = std::thread(&Window::windowHandler, this);
	update_frame = true;
}

void Window::windowHandler() {
	window_mutex.lock();
	window.create(sf::VideoMode(window_width, window_height), window_title, sf::Style::Resize | sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close);
	screen_view = window.getView();

	window_mutex.unlock();
	while (running) {
		window_mutex.lock();
		//check input
		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::Resized:
				window_width = e.size.width;//window.getSize().x;
				window_height = e.size.height;//window.getSize().y;
				screen_view = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(window_width), static_cast<float>(window_height)));
				update_frame = true;
				break;
			case sf::Event::MouseWheelScrolled:
				if (e.mouseWheelScroll.delta > 0.f) {
					zoomViewAtPixel({ e.mouseWheelScroll.x, e.mouseWheelScroll.y }, world_view, window, 1.f / mouse_zoom_amount);
				}
				else if (e.mouseWheelScroll.delta < 0.f) {
					zoomViewAtPixel({ e.mouseWheelScroll.x, e.mouseWheelScroll.y }, world_view, window, mouse_zoom_amount);
				}
				update_frame = true;
				break;
			case sf::Event::MouseMoved:
				mouse_pos = { e.mouseMove.x, e.mouseMove.y };
				if (mouse_move) {
					if (mouse_left_bounce) {
						mouse_current_pos = window.mapPixelToCoords(mouse_pos, world_view);
						sf::Vector2f m = mouse_start_pos - mouse_current_pos;
						world_view.move(m);
					}
					update_frame = true;
				}
				break;
			case sf::Event::MouseButtonPressed:
				mouse_pos = { e.mouseButton.x, e.mouseButton.y };
				mouse_left_down = true;
				if (mouse_move) {
					if (diagram_area.contains(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y))) {
						if (!mouse_left_bounce) {
							mouse_start_pos = window.mapPixelToCoords(mouse_pos, world_view);
							mouse_left_bounce = true;
						}
					}
				}
				break;
			case sf::Event::MouseButtonReleased:
				mouse_pos = { e.mouseButton.x, e.mouseButton.y };
				mouse_left_down = false;
				mouse_left_bounce = false;
				break;
			default:
				break;
			}
		}

		if (update_frame) {
			window.clear(window_background_color);

			window.setTitle(window_title);
			window.setSize(sf::Vector2u(window_width, window_height));

			updateView();

			renderLines();
			
			renderDrawObject();

			renderUI();
			
			window.display();
			update_frame = false;
		}
		window_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(update_interval));
	}
	//kill window
	window.close();
}

void Window::setMouseZoom(bool v) {
	window_mutex.lock();
	mouse_zoom = v;
	window_mutex.unlock();
}

void Window::setMouseMove(bool v) {
	window_mutex.lock();
	mouse_move = v;
	window_mutex.unlock();
}

void GeometryDisplay::zoomViewAtPixel(sf::Vector2i pixel, sf::View & view, sf::RenderWindow & window, float zoom) {
	const sf::Vector2f beforeCoord = window.mapPixelToCoords(pixel, view);
	view.zoom(zoom);
	const sf::Vector2f afterCoord = window.mapPixelToCoords(pixel, view);
	const sf::Vector2f offsetCoords = beforeCoord - afterCoord;
	view.move(offsetCoords);
}

void Window::updateView() {
	diagram_area.left = ui_border_thickness;
	diagram_area.top = ui_border_thickness;
	diagram_area.width = screen_view.getSize().x - ui_border_thickness * 2;
	diagram_area.height = screen_view.getSize().y - ui_border_thickness * 2;
}

void Window::renderUI() {
	//render border rectangles
	ui_vertex_array.clear();
	std::vector<wykobi::rectangle<float>> rect_vec(4);
	float win_width = screen_view.getSize().x;
	float win_height = screen_view.getSize().y;
	//top
	rect_vec[0] = (wykobi::make_rectangle(0.f, 0.f, win_width, ui_border_thickness));
	//bottom
	rect_vec[1] = (wykobi::make_rectangle(0.f, win_height - ui_border_thickness, win_width, win_height));
	//left
	rect_vec[2] = (wykobi::make_rectangle(0.f, ui_border_thickness, ui_border_thickness, win_height - ui_border_thickness));
	//right
	rect_vec[3] = (wykobi::make_rectangle(win_width - ui_border_thickness, ui_border_thickness, win_width, win_height - ui_border_thickness));
	for (wykobi::rectangle<float> & rect : rect_vec) {
		wykobi::polygon<float, 2> poly = wykobi::make_polygon(rect);
		std::vector<wykobi::triangle<float, 2>> tri_vec;
		wykobi::algorithm::polygon_triangulate<wykobi::point2d<float>>(poly, std::back_inserter(tri_vec));
		for (wykobi::triangle<float, 2> & tri : tri_vec) {
			for (std::size_t i = 0; i < tri.size(); ++i) {
				sf::Vertex v;
				v.position = sf::Vector2f(tri[i].x, tri[i].y);
				v.color = ui_border_color;
				ui_vertex_array.append(v);
			}
		}
	}
	window.setView(screen_view);
	window.draw(ui_vertex_array);
}

float GeometryDisplay::getClosestPointInRes(float v, float res) {
	return v - std::fmod(v, res);
}

wykobi::rectangle<float> GeometryDisplay::getBoundingRectangle(wykobi::polygon<float, 2> & poly) {
	wykobi::point2d<float> low_point = poly[0];
	wykobi::point2d<float> high_point = poly[0];
	for (std::size_t i = 1; i < poly.size(); ++i) {
		if (poly[i].x < low_point.x) {
			low_point.x = poly[i].x;
		}
		if (poly[i].y < low_point.y) {
			low_point.y = poly[i].y;
		}
		if (poly[i].x > high_point.x) {
			high_point.x = poly[i].x;
		}
		if (poly[i].y > high_point.y) {
			high_point.y = poly[i].y;
		}
	}
	return wykobi::make_rectangle(low_point, high_point);
}

void Window::renderLines() {
	diagram_vertex_array.clear();
	diagram_text_vector.clear();
	
	wykobi::polygon<float, 2> world_poly(4);
	sf::Vector2f p;
	p = window.mapPixelToCoords(sf::Vector2i((int)diagram_area.left, (int)diagram_area.top), world_view);
	world_poly[0] = wykobi::make_point(p.x, p.y);
	p = window.mapPixelToCoords(sf::Vector2i((int)diagram_area.left + (int)diagram_area.width, (int)diagram_area.top), world_view);
	world_poly[1] = wykobi::make_point(p.x, p.y);
	p = window.mapPixelToCoords(sf::Vector2i((int)diagram_area.left + (int)diagram_area.width, (int)diagram_area.top + (int)diagram_area.height), world_view);
	world_poly[2] = wykobi::make_point(p.x, p.y);
	p = window.mapPixelToCoords(sf::Vector2i((int)diagram_area.left, (int)diagram_area.top + (int)diagram_area.height), world_view);
	world_poly[3] = wykobi::make_point(p.x, p.y);

	wykobi::rectangle<float> bounding_rect = getBoundingRectangle(world_poly);

	float x, y;
	float max_x, max_y;

	max_x = bounding_rect[1].x;
	max_y = bounding_rect[1].y;
	
	std::vector<wykobi::segment<float, 2>> vertical_segments;
	std::vector<wykobi::segment<float, 2>> horizontal_segments;

	//vertical lines
	x = getClosestPointInRes(bounding_rect[0].x, diagram_line_resolution.x);
	y = bounding_rect[0].y;
	while (x < max_x) {
		wykobi::segment<float, 2> seg = wykobi::make_segment(x, y, x, max_y);
		if (segmentIntersectPolygon(seg, world_poly)) {
			wykobi::segment<float, 2> draw_seg;
			bool first = false;
			for (std::size_t i = 0; i < world_poly.size(); ++i) {
				wykobi::segment<float, 2> test_seg = wykobi::edge(world_poly, i);
				if (wykobi::intersect(seg, test_seg)) {
					if (!first) {
						draw_seg[0] = wykobi::intersection_point(seg, test_seg);
						first = true;
					}
					else {
						draw_seg[1] = wykobi::intersection_point(seg, test_seg);
						break;
					}
				}
			}

			vertical_segments.push_back(draw_seg);

			for (std::size_t i = 0; i < draw_seg.size(); ++i) {
				sf::Vertex v;
				v.color = diagram_line_color;
				v.position.x = seg[i].x;
				v.position.y = seg[i].y;
				diagram_vertex_array.append(v);
			}
		}
		x += diagram_line_resolution.x;
	}
	//horizontal lines
	x = bounding_rect[0].x;
	y = getClosestPointInRes(bounding_rect[0].y, diagram_line_resolution.y);
	while (y < max_y) {
		wykobi::segment<float, 2> seg = wykobi::make_segment(x, y, max_x, y);
		if (segmentIntersectPolygon(seg, world_poly)) {
			wykobi::segment<float, 2> draw_seg;
			bool first = false;
			for (std::size_t i = 0; i < world_poly.size(); ++i) {
				wykobi::segment<float, 2> test_seg = wykobi::edge(world_poly, i);
				if (wykobi::intersect(seg, test_seg)) {
					if (!first) {
						draw_seg[0] = wykobi::intersection_point(seg, test_seg);
						first = true;
					}
					else {
						draw_seg[1] = wykobi::intersection_point(seg, test_seg);
						break;
					}
				}
			}
			horizontal_segments.push_back(draw_seg);
			for (std::size_t i = 0; i < draw_seg.size(); ++i) {
				sf::Vertex v;
				v.color = diagram_line_color;
				v.position.x = seg[i].x;
				v.position.y = seg[i].y;
				diagram_vertex_array.append(v);
			}
		}
		y += diagram_line_resolution.y;
	}

	//text
	window.setView(world_view);
	window.draw(diagram_vertex_array);
	window.setView(screen_view);
	for (auto & seg : vertical_segments) {
		std::size_t index = (seg[0].y < seg[1].y) ? 0 : 1;
		sf::Text t;
		t.setFillColor(diagram_text_color);
		t.setPosition(sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(seg[index].x, seg[index].y), world_view)));
		std::ostringstream s;
		s << seg[index].x;
		t.setFont(*text_font);
		t.setString(s.str());
		t.setCharacterSize(diagram_text_char_size);
		window.draw(t);
	}
	for (auto & seg : horizontal_segments) {
		std::size_t index = (seg[0].x < seg[1].x) ? 0 : 1;
		sf::Text t;
		t.setFillColor(diagram_text_color);
		t.setPosition(sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(seg[index].x, seg[index].y), world_view)));
		std::ostringstream s;
		s << seg[index].y;
		t.setFont(*text_font);
		t.setString(s.str());
		t.setCharacterSize(diagram_text_char_size);
		window.draw(t);
	}
}

void Window::renderDrawObject() {
	//render shapes
	draw_object_vec_mutex.lock();
	draw_object_vertex_array.clear();
	for (auto it = draw_object_vec.begin(); it != draw_object_vec.end(); ++it) {
		(*it)->appendVertex(draw_object_vertex_array);
	}
	draw_object_vec_mutex.unlock();
	window.setView(world_view);
	window.draw(draw_object_vertex_array);
}

void Window::setTitle(std::string title) {
	window_mutex.lock();
	window_title = title;
	update_frame = true;
	window_mutex.unlock();
}

void Window::setUpdateInterval(int t) {
	window_mutex.lock();
	update_interval = t;
	window_mutex.unlock();
}

void Window::setSize(int w, int h) {
	window_mutex.lock();
	window_width = w;
	window_height = h;
	update_frame = true;
	window_mutex.unlock();
}

void Window::setDiagramPosition(float x, float y) {
	window_mutex.lock();
	world_view.setCenter(x + world_view.getSize().x / 2, y + world_view.getSize().y / 2);
	update_frame = true;
	window_mutex.unlock();
}

void Window::rotateDiagram(float r) {
	window_mutex.lock();
	world_view.rotate(r);
	update_frame = true;
	window_mutex.unlock();
}

void Window::setDiagramLineResolution(float x, float y) {
	window_mutex.lock();
	diagram_line_resolution.x = x;
	diagram_line_resolution.y = y;
	update_frame = true;
	window_mutex.unlock();
}

void Window::addShape(DrawObject & shape) {
	draw_object_vec_mutex.lock();
	draw_object_vec.push_back(std::unique_ptr<DrawObject>(shape.clone()));
	update_frame = true;
	draw_object_vec_mutex.unlock();
}

void Window::addShape(wykobi::polygon<float, 2> poly) {
	PolygonShape shape(poly);
	addShape(shape);
}

void Window::addShape(wykobi::segment<float, 2> seg) {
	LineShape shape(seg);
	addShape(shape);
}

void Window::clearShapeVec() {
	draw_object_vec.clear();
	update_frame = true;
}

void Window::close() {
	running = false;
	join();
}

void Window::join() {
	if (window_thread.joinable()) {
		window_thread.join();
	}
}

PolygonShape::PolygonShape(wykobi::polygon<float, 2> poly) {
	polygon = poly;
}

PolygonShape* GeometryDisplay::PolygonShape::clone() {
	return new PolygonShape(*this);
}

void PolygonShape::appendVertex(sf::VertexArray & vertex_arr) {
	if (inner_fill) {
		std::vector<wykobi::triangle<float, 2>> triangle_vec;
		wykobi::algorithm::polygon_triangulate<wykobi::point2d<float>>(polygon, std::back_inserter(triangle_vec));
		for (wykobi::triangle<float, 2> & tri : triangle_vec) {
			for (std::size_t i = 0; i < tri.size(); ++i) {
				sf::Vertex v;
				v.position = sf::Vector2f(tri[i].x, tri[i].y);
				v.color = fill_color;
				vertex_arr.append(v);
			}
		}
	}
	if (outer_line) {
		for (std::size_t i = 0; i < polygon.size(); ++i) {
			wykobi::segment<float, 2> seg = wykobi::edge(polygon, i);
			for (wykobi::triangle<float, 2> & tri : makeTriangleLine(seg, outer_line_thickness)) {
				for (std::size_t j = 0; j < tri.size(); ++j) {
					sf::Vertex v;
					v.position = sf::Vector2f(tri[j].x, tri[j].y);
					v.color = line_color;
					vertex_arr.append(v);
				}
			}
		}
	}
}
LineShape::LineShape(wykobi::segment<float, 2> seg) {
	segment = seg;
}

LineShape* LineShape::clone() {
	return new LineShape(*this);
}

void LineShape::appendVertex(sf::VertexArray & vertex_arr) {
	if (inner_fill) {
		for (wykobi::triangle<float, 2> & tri : makeTriangleLine(segment, thickness)) {
			for (std::size_t i = 0; i < tri.size(); ++i) {
				sf::Vertex v;
				v.position = sf::Vector2f(tri[i].x, tri[i].y);
				v.color = fill_color;
				vertex_arr.append(v);
			}
		}
	}
}

std::vector<wykobi::triangle<float, 2>> GeometryDisplay::makeTriangleLine(float x0, float y0, float x1, float y1, float thickness) {
	wykobi::segment<float, 2> segment = wykobi::make_segment(x0, y0, x1, y1);
	float length = wykobi::distance(segment);
	float dx = segment[0].x - segment[1].x;
	float dy = segment[0].y - segment[1].y;
	dx /= length;
	dy /= length;
	wykobi::point2d<float> p0 = wykobi::make_point(segment[0].x + (thickness / 2)*dy, segment[0].y - (thickness / 2)*dx);
	wykobi::point2d<float> p1 = wykobi::make_point(segment[0].x - (thickness / 2)*dy, segment[0].y + (thickness / 2)*dx);
	wykobi::point2d<float> p2 = wykobi::make_point(segment[1].x - (thickness / 2)*dy, segment[1].y + (thickness / 2)*dx);
	wykobi::point2d<float> p3 = wykobi::make_point(segment[1].x + (thickness / 2)*dy, segment[1].y - (thickness / 2)*dx);
	wykobi::polygon<float, 2> poly = wykobi::make_polygon(std::vector<wykobi::point2d<float>>{p0, p1, p2, p3});
	std::vector<wykobi::triangle<float, 2>> triangle_vec;
	wykobi::algorithm::polygon_triangulate<wykobi::point2d<float>>(poly, std::back_inserter(triangle_vec));
	return triangle_vec;
}

std::vector<wykobi::triangle<float, 2>>GeometryDisplay::makeTriangleLine(wykobi::segment<float, 2> & seg, float thickness) {
	return makeTriangleLine(seg[0].x, seg[0].y, seg[1].x, seg[1].y, thickness);
}

bool GeometryDisplay::segmentIntersectPolygon(wykobi::segment<float, 2> & seg, wykobi::polygon<float, 2> & poly) {
	for (std::size_t i = 0; i < poly.size(); ++i) {
		if (wykobi::intersect(wykobi::edge(poly, i), seg)) {
			return true;
		}
	}
	return false;
}

//float normalizeFloat(float value, float min, float max) {
//	return (value - min) / (max - min);
//}
//float deNormalizeFloat(float value, float min, float max) {
//	return (value * (max - min) + min);
//}

//end