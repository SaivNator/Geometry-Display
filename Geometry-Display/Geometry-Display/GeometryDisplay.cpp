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
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(window_width), static_cast<float>(window_height))));
				update_frame = true;
				break;
			default:
				break;
			}
		}
		//check settings
		if (update_settings) {
			update_settings = false;
		}
		if (update_frame) {
			window.clear();

			updateDiagram();
			
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

void Window::updateDiagram() {
	float win_width = static_cast<float>(window_width);
	float win_height = static_cast<float>(window_height);
	diagram_screen_area[0].x = ui_border_thickness;
	diagram_screen_area[0].y = ui_border_thickness;
	diagram_screen_area[1].x = win_width - ui_border_thickness;
	diagram_screen_area[1].y = win_height - ui_border_thickness;


	wykobi::rectangle<float> temp_rect;
	float world_width = (diagram_screen_area[1].x - diagram_screen_area[0].x) * diagram_world_zoom.x;
	float world_height = (diagram_screen_area[1].y - diagram_screen_area[0].y) * diagram_world_zoom.y;
	temp_rect[0].x = diagram_position.x;
	temp_rect[0].y = diagram_position.y;
	temp_rect[1].x = diagram_position.x + world_width;
	temp_rect[1].y = diagram_position.y + world_height;
	diagram_world_area = wykobi::rotate(diagram_world_rotation, wykobi::make_polygon(temp_rect));


	diagram_screen_rotation = 90.f * diagram_origin_corner;
	
}

void Window::renderUI() {
	//render border rectangles
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
	window.draw(ui_vertex_array);
}

void Window::renderDiagram() {
	diagram_vertex_array.clear();
	
	//horizontal lines

	

	//vertical lines


}

void Window::renderDrawObject() {
	//render shapes
	draw_object_vec_mutex.lock();
	draw_object_vertex_array.clear();
	for (auto it = draw_object_vec.begin(); it != draw_object_vec.end(); ++it) {
		(*it)->appendVertex(draw_object_vertex_array);
	}
	draw_object_vec_mutex.unlock();

	sf::RenderStates states;

	wykobi::point2d<float> corner = wykobi::rectangle_corner(diagram_screen_area, diagram_origin_corner);

	states.transform.translate(corner.x, corner.y);
	states.transform.translate(-diagram_position.x, -diagram_position.y);
	states.transform.scale(diagram_world_zoom.x, diagram_world_zoom.y);
	states.transform.rotate(diagram_world_rotation);
	states.transform.rotate(diagram_screen_rotation);

	window.draw(draw_object_vertex_array, states);
}

void Window::setTitle(std::string & title) {
	window_mutex.lock();
	window.setTitle(title);
	update_frame = true;
	window_mutex.unlock();
}

void Window::setDiagramPosition(float x, float y) {
	window_mutex.lock();
	diagram_position.x = x;
	diagram_position.y = y;
	update_frame = true;
	window_mutex.unlock();
}

void Window::setDiagramOriginCorner(std::size_t i) {
	window_mutex.lock();
	diagram_origin_corner = i;
	update_frame = true;
	window_mutex.unlock();
}

void Window::setDiagramRotaton(float r) {
	window_mutex.lock();
	diagram_world_rotation = r;
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