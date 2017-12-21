//Author: Sivert Andresen Cubedo

#include "GeometryDisplay.hpp"

#include <cmath>

using namespace GeometryDisplay;

Window::Window() { 
	
}

void Window::create() {
	//std::cout << ">>>" << std::this_thread::get_id() << "<<<: " << "init()\n";
	window_thread = std::thread(&Window::windowHandler, this);
	update_frame = true;
}

void Window::windowHandler() {

	window.create(sf::VideoMode(window_width, window_height), "Default title", sf::Style::Resize | sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close);

	while (running) {
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
				//window.setSize(sf::Vector2u(window_width, window_height));
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(window_width), static_cast<float>(window_height))));

				update_frame = true;
				break;
			default:
				break;
			}
		}

		//check settings
		if (update_settings) {
			//window.setSize(sf::Vector2u(window_width, window_height));
			update_settings = false;
		}

		if (update_frame) {
			renderFrame();
			update_frame = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(update_interval));
	}
	//kill window
	window.close();
}

void Window::renderUI() {
	sf::View view = window.getDefaultView();

	//make border rectangles
	ui_vertex_array.clear();
	std::vector<wykobi::rectangle<float>> rect_vec(4);
	float win_width = static_cast<float>(window_width);
	float win_height = static_cast<float>(window_height);
	//top
	rect_vec[0] = (wykobi::make_rectangle(0.f, 0.f, win_width, ui_border_thickness));
	//bottom
	rect_vec[1] = (wykobi::make_rectangle(0.f, window_height - ui_border_thickness, win_width, win_height));
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
}

void Window::renderFrame() {
	window.clear();

	//render shapes
	shape_vec_mutex.lock();
	shape_vertex_array.clear();
	for (auto it = shape_vec.begin(); it != shape_vec.end(); ++it) {
		(*it)->appendVertex(shape_vertex_array);
	}
	shape_vec_mutex.unlock();

	


	window.draw(shape_vertex_array);

	renderUI();

	window.draw(ui_vertex_array);

	window.display();
}

void Window::addShape(DrawObject & shape) {
	shape_vec_mutex.lock();
	shape_vec.push_back(std::unique_ptr<DrawObject>(shape.clone()));
	update_frame = true;
	shape_vec_mutex.unlock();
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
	shape_vec.clear();
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
	float length = wykobi::distance(segment);
	float dx = segment[0].x - segment[1].x;
	float dy = segment[0].y - segment[1].y;
	dx /= length;
	dy /= length;
	wykobi::point2d<float> p1 = wykobi::make_point(segment[0].x + (thickness / 2)*dy, segment[0].y - (thickness / 2)*dx);
	wykobi::point2d<float> p2 = wykobi::make_point(segment[0].x - (thickness / 2)*dy, segment[0].y + (thickness / 2)*dx);
	wykobi::point2d<float> p3 = wykobi::make_point(segment[1].x - (thickness / 2)*dy, segment[1].y + (thickness / 2)*dx);
	wykobi::point2d<float> p4 = wykobi::make_point(segment[1].x + (thickness / 2)*dy, segment[1].y - (thickness / 2)*dx);
	wykobi::polygon<float, 2> poly = wykobi::make_polygon(std::vector<wykobi::point2d<float>>{p1, p2, p3, p4});
	std::vector<wykobi::triangle<float, 2>> triangle_vec;
	wykobi::algorithm::polygon_triangulate<wykobi::point2d<float>>(poly, std::back_inserter(triangle_vec));
	for (wykobi::triangle<float, 2> & tri : triangle_vec) {
		for (std::size_t i = 0; i < tri.size(); ++i) {
			sf::Vertex v;
			v.position = sf::Vector2f(tri[i].x, tri[i].y);
			v.color = color;
			vertex_arr.append(v);
		}
	}
}

//end