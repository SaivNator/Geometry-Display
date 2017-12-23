//Author: Sivert Andresen Cubedo

#include "GeometryDisplay.hpp"

using namespace GeometryDisplay;

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
	//std::cout << ">>>" << std::this_thread::get_id() << "<<<: " << "init()\n";
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
			case sf::Event::MouseMoved:
				break;
			case sf::Event::MouseButtonPressed:
				mouse_left_down = true;
				break;
			case sf::Event::MouseButtonReleased:
				mouse_left_down = false;
				break;
			default:
				break;
			}
		}
		//check settings
		if (update_settings) {
			window.setTitle(window_title);
			window.setSize(sf::Vector2u(window_width, window_height));
			update_settings = false;
		}

		if (mouse_move) {
			updateMouseMove();
			update_frame = true;
		}

		if (update_frame) {
			window.clear();

			updateView();
			
			renderDrawObject();

			renderUI();

			renderLines();
			
			window.display();
			update_frame = false;
		}
		window_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(update_interval));
	}
	//kill window
	window.close();
}

void Window::setMouseMove(bool v) {
	mouse_move = v;
}

float normalizeFloat(float value, float min, float max) {
	return (value - min) / (max - min);
}
float deNormalizeFloat(float value, float min, float max) {
	return (value * (max - min) + min);
}

void Window::updateMouseMove() {
	
	if (mouse_left_down && !mouse_left_bounce) {
		mouse_start_pos = window.mapPixelToCoords(sf::Mouse::getPosition(), world_view);
		mouse_left_bounce = true;
	}
	else if (mouse_left_down && mouse_left_bounce) {
		mouse_current_pos = window.mapPixelToCoords(sf::Mouse::getPosition(), world_view);

		sf::Vector2f m = mouse_start_pos - mouse_current_pos;

		world_view.move(m);



	}
	else if (!mouse_left_down && mouse_left_bounce) {
		mouse_left_bounce = false;
	}
	
}

void Window::updateView() {

	

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

void Window::renderLines() {
	diagram_vertex_array.clear();
	diagram_text_vector.clear();
	
	sf::RenderStates line_states;
	
	/*
	wykobi::point2d<float> corner = wykobi::rectangle_corner(diagram_screen_area, diagram_screen_origin_corner);
	line_states.transform.translate(corner.x, corner.y);
	line_states.transform.rotate(diagram_screen_rotation);
	line_states.transform.translate(-diagram_position.x, -diagram_position.y);
	*/

	/*
	float x, y;
	float max_x, max_y;
	max_x = diagram_position.x + diagram_world_size.x;
	max_y = diagram_position.y + diagram_world_size.y;
	//vertical lines
	x = getClosestPointInRes(diagram_position.x, diagram_line_resolution.x);
	y = diagram_position.y;
	while (x < max_x) {
		for (auto tri : makeTriangleLine(x, y, x, max_y, diagram_line_thickness)) {
			for (std::size_t i = 0; i < tri.size(); ++i) {
				sf::Vertex v;
				v.position.x = tri[i].x;
				v.position.y = tri[i].y;
				v.color = diagram_line_color;
				diagram_vertex_array.append(v);
			}
		}

		sf::Text line_pos_text;
		line_pos_text.setFont(*text_font);
		line_pos_text.setPosition(x, y - 20);
		line_pos_text.setCharacterSize(diagram_text_char_size);
		std::ostringstream t;
		t << x;
		line_pos_text.setString(t.str());
		diagram_text_vector.push_back(line_pos_text);

		x += diagram_line_resolution.x;
	}
	//horizontal lines
	x = diagram_position.x;
	y = getClosestPointInRes(diagram_position.y, diagram_line_resolution.y);
	while (y < max_y) {
		for (auto tri : makeTriangleLine(x, y, max_x, y, diagram_line_thickness)) {
			for (std::size_t i = 0; i < tri.size(); ++i) {
				sf::Vertex v;
				v.position.x = tri[i].x;
				v.position.y = tri[i].y;
				v.color = diagram_line_color;
				diagram_vertex_array.append(v);
			}
		}

		sf::Text line_pos_text;
		line_pos_text.setFont(*text_font);
		line_pos_text.setPosition(x - 20, y);
		line_pos_text.setCharacterSize(diagram_text_char_size);
		std::ostringstream t;
		t << y;
		line_pos_text.setString(t.str());
		diagram_text_vector.push_back(line_pos_text);

		y += diagram_line_resolution.y;
	}

	//text

	window.draw(diagram_vertex_array, line_states);
	for (auto & t : diagram_text_vector) {
		t.setRotation(-diagram_screen_rotation);
		window.draw(t, line_states);
	}
	*/
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
	update_settings = true;
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
	update_settings = true;
	update_frame = true;
	window_mutex.unlock();
}

void Window::setDiagramPosition(float x, float y) {
	window_mutex.lock();
	world_view.setCenter(x + world_view.getSize().x / 2 , y + world_view.getSize().y / 2);
	update_frame = true;
	window_mutex.unlock();
}

void Window::setDiagramOriginCorner(int i) {
	window_mutex.lock();
	origin_corner = i;
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
	std::vector<wykobi::triangle<float, 2>> triangle_vec;
	wykobi::algorithm::polygon_triangulate<wykobi::point2d<float>>(polygon, std::back_inserter(triangle_vec));
	for (wykobi::triangle<float, 2> & tri : triangle_vec) {
		for (std::size_t i = 0; i < tri.size(); ++i) {
			sf::Vertex v;
			v.position = sf::Vector2f(tri[i].x, tri[i].y);
			v.color = color;
			vertex_arr.append(v);
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
	for (wykobi::triangle<float, 2> & tri : makeTriangleLine(segment, thickness)) {
		for (std::size_t i = 0; i < tri.size(); ++i) {
			sf::Vertex v;
			v.position = sf::Vector2f(tri[i].x, tri[i].y);
			v.color = color;
			vertex_arr.append(v);
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

//end