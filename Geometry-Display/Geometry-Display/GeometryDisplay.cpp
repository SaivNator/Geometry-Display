//Author: Sivert Andresen Cubedo

#include "GeometryDisplay.hpp"


void GeometryDisplay::Window::create() {
	//std::cout << ">>>" << std::this_thread::get_id() << "<<<: " << "init()\n";
	window_thread = std::thread(&Window::windowHandler, this);
}

void GeometryDisplay::Window::windowHandler() {

	window.create(sf::VideoMode(window_width, window_height), "Default title", sf::Style::Default);

	while (running) {
		//check input
		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				running = false;
				break;
			default:
				break;
			}
		}

		//check settings
		if (update_settings) {
			window.setSize(sf::Vector2u(window_width, window_height));
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

void GeometryDisplay::Window::renderFrame() {
	window.clear();
	shape_vertex_array.clear();
	for (auto it = shape_vec.begin(); it != shape_vec.end(); ++it) {
		it->get().appendVertex(shape_vertex_array);
	}
	window.draw(shape_vertex_array);
	window.display();
}

void GeometryDisplay::Window::addShape(Shape & shape) {
	shape_vec.push_back(shape);
	update_frame = true;
}

//std::vector GeometryDisplay::Window::addShape(std::vector<wykobi::polygon<float, 2>> & vec) {
//	update_frame = true;
//}

void GeometryDisplay::Window::clearShapeVec() {
	shape_vec.clear();
	update_frame = true;
}

void GeometryDisplay::Window::close() {
	running = false;
	join();
}

void GeometryDisplay::Window::join() {
	if (window_thread.joinable()) {
		window_thread.join();
	}
}

GeometryDisplay::PolygonShape::PolygonShape(wykobi::polygon<float, 2> poly) {
	polygon = poly;
}

void GeometryDisplay::PolygonShape::appendVertex(sf::VertexArray & vertex_arr) {
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

//end