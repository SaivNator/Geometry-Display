//Author: Sivert Andresen Cubedo

#include "GeometryDisplay.hpp"

using namespace GeometryDisplay;



void UIPosition::positionOver(UIPosition & parent) {
	this->area.left = parent.area.left;
	this->area.top = parent.area.top - this->area.height;
}
void UIPosition::positionUnder(UIPosition & parent) {
	this->area.left = parent.area.left;
	this->area.top = parent.area.top + parent.area.height;
}
void UIPosition::positionLeft(UIPosition & parent) {
	this->area.left = parent.area.left - this->area.width;
	this->area.top = parent.area.top;
}
void UIPosition::positionRight(UIPosition & parent) {
	this->area.left = parent.area.left + parent.area.width;
	this->area.top = parent.area.top;
}
sf::Vector2i UIPosition::getSize() {
	return { area.width, area.height };
}

sf::IntRect UIPosition::getArea() {
	return area;
}

sf::Vector2i UIPosition::getPosition() {
	return { area.left, area.top };
}

void Button::setFont(std::shared_ptr<sf::Font> ptr) {
	text_font = ptr;
}

void Button::setArea(sf::IntRect button_area) {
	area = button_area;
}

void PushButton::forcePush() {
	force_push = true;
}

void PushButton::click(const sf::Vector2i & mouse_pos) {
	if (!bounce) {
		if (area.contains(mouse_pos)) {
			is_clicked = true;
		}
	}
}

bool PushButton::getState() {
	if (force_push) {
		force_push = false;
		return true;
	}
	else if (is_clicked && !bounce) {
		bounce = true;
		return true;
	}
	else {
		return false;
	}
}

void PushButton::release() {
	is_clicked = false;
	bounce = false;
}

void PushButton::draw(sf::RenderTarget & target, sf::RenderStates states) const {
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
			if (is_clicked) {
				v.color = click_color;
			}
			else {
				v.color = not_click_color;
			}
			vertex_array.append(v);
		}
	}
	text.setFont(*text_font);
	text.setPosition((float)area.left, (float)area.top);
	text.setCharacterSize(text_char_size);
	if (is_clicked) {
		text.setString(click_text);
		text.setFillColor(click_text_color);
	}
	else {
		text.setString(not_click_text);
		text.setFillColor(not_click_text_color);
	}
	target.draw(vertex_array, states);
	target.draw(text, states);
}

void ToggleButton::click(const sf::Vector2i & mouse_pos) {
	if (!bounce) {
		if (area.contains(mouse_pos)) {
			toggle = !toggle;
			bounce = true;
		}
	}
}

void ToggleButton::setToggle(bool v) {
	toggle = v;
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
	text.setFont(*text_font);
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

Window::Window(std::shared_ptr<sf::Font> font_ptr) {
	text_font = font_ptr;
}

Window::Window() { 
	text_font = std::shared_ptr<sf::Font>(new sf::Font());
	if (!text_font->loadFromFile("fonts/arial.ttf")) {
		std::cout << "Font load failed\n";
	}
}

void Window::create() {
	window_thread = std::thread(&Window::windowHandler, this);

	//init move button
	mouse_move_button.setFont(text_font);
	mouse_move_button.not_toggle_text = "Move\nMode";
	mouse_move_button.toggle_text = "Move\nMode";
	mouse_move_button.setArea(sf::IntRect(0, 0, 30, 30));
	mouse_move_button.not_toggle_color = { 204, 204, 204 };
	mouse_move_button.toggle_color = { 91, 105, 233 };

	//init show shape text button
	show_draw_object_button.setFont(text_font);
	show_draw_object_button.not_toggle_text = "Show\nName";
	show_draw_object_button.toggle_text = "Hide\nName";
	show_draw_object_button.setArea(sf::IntRect(0, 0, 30, 30));
	show_draw_object_button.positionRight(mouse_move_button);
	show_draw_object_button.not_toggle_color = { 204, 204, 204 };
	show_draw_object_button.toggle_color = { 91, 105, 233 };

	//init lock button
	lock_world_view_scale_button.setFont(text_font);
	lock_world_view_scale_button.not_toggle_text = "Lock\nScale";
	lock_world_view_scale_button.toggle_text = "Free\nScale";
	lock_world_view_scale_button.setArea(sf::IntRect(0, 0, 30, 30));
	lock_world_view_scale_button.positionRight(show_draw_object_button);
	lock_world_view_scale_button.not_toggle_color = { 204, 204, 204 };
	lock_world_view_scale_button.toggle_color = { 91, 105, 233 };

	//init auto size button
	auto_size_button.setFont(text_font);
	auto_size_button.not_click_text = "Auto\nSize";
	auto_size_button.click_text = "Auto\nSize";
	auto_size_button.setArea(sf::IntRect(0, 0, 30, 30));
	auto_size_button.positionRight(lock_world_view_scale_button);
	auto_size_button.not_click_color = { 204, 204, 204 };
	auto_size_button.click_color = { 91, 105, 233 };

	update_frame = true;
}

void Window::create(sf::Vector2u win_size) {
	window_size = win_size;
	create();
}

void Window::windowHandler() {
	window_mutex.lock();
	window.create(sf::VideoMode(window_size.x, window_size.y), window_title, sf::Style::Resize | sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close);
	screen_view = window.getView();
	world_view = sf::View({ 0.f, 0.f, 100.f, 100.f });
	updateView();
	setViewPositionCorner(world_view, { 0.f, 0.f }, 0);

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
				window_size = { e.size.width, e.size.height };
				screen_view = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(window_size.x), static_cast<float>(window_size.y)));
				update_frame = true;
				break;
			case sf::Event::MouseWheelScrolled:
				if (mouse_move_button.getState()) {
					if (e.mouseWheelScroll.delta > 0.f) {
						zoomViewAtPixel({ e.mouseWheelScroll.x, e.mouseWheelScroll.y }, world_view, window, 1.f / mouse_zoom_amount);
					}
					else if (e.mouseWheelScroll.delta < 0.f) {
						zoomViewAtPixel({ e.mouseWheelScroll.x, e.mouseWheelScroll.y }, world_view, window, mouse_zoom_amount);
					}
					update_frame = true;
				}
				break;
			case sf::Event::MouseMoved:
				mouse_pos = { e.mouseMove.x, e.mouseMove.y };
				if (mouse_move_button.getState()) {
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
				mouse_move_button.click(mouse_pos);
				show_draw_object_button.click(mouse_pos);
				lock_world_view_scale_button.click(mouse_pos);
				auto_size_button.click(mouse_pos);
				mouse_left_down = true;
				if (mouse_move_button.getState()) {
					if (diagram_area.contains(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y))) {
						if (!mouse_left_bounce) {
							mouse_start_pos = window.mapPixelToCoords(mouse_pos, world_view);
							mouse_left_bounce = true;
						}
					}
				}
				if (auto_size_button.getState()) {
					autoSize();
				}
				break;
			case sf::Event::MouseButtonReleased:
				mouse_pos = { e.mouseButton.x, e.mouseButton.y };
				mouse_left_down = false;
				mouse_left_bounce = false;
				mouse_move_button.release();
				show_draw_object_button.release();
				lock_world_view_scale_button.release();
				auto_size_button.release();
				update_frame = true;
				break;
			default:
				break;
			}
		}

		if (update_frame) {
			window.clear(window_background_color);

			window.setTitle(window_title);

			window.setSize(window_size);

			updateView();

			renderUI();

			renderLines();
			
			renderDrawObject();

			window.setView(screen_view);
			window.draw(mouse_move_button);
			window.draw(show_draw_object_button);
			window.draw(lock_world_view_scale_button);
			window.draw(auto_size_button);
			
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
	window_mutex.lock();
	mouse_move_button.setToggle(v);
	window_mutex.unlock();
}

void Window::setLockScreenScale(bool v) {
	window_mutex.lock();
	lock_world_view_scale_button.setToggle(v);
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
diagram_area.width = static_cast<float>(window_size.x) - ui_border_thickness * 2;
diagram_area.height = static_cast<float>(window_size.y) - ui_border_thickness * 2;
world_view.setViewport(sf::FloatRect(
	normalize(diagram_area.left, 0.f, (float)window_size.x),
	normalize(diagram_area.top, 0.f, (float)window_size.y),
	normalize(diagram_area.width, 0.f, (float)window_size.x),
	normalize(diagram_area.height, 0.f, (float)window_size.y)
));
//autoLineResolution();
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

//void Window::autoLineResolution() {
//	sf::Vector2f v0 = window.mapPixelToCoords({ 0, 0 }, world_view);
//	sf::Vector2f v1 = window.mapPixelToCoords(line_screen_distance, world_view);
//	sf::Vector2f v_dist = v1 - v0;
//	diagram_line_resolution.x = v_dist.x;//std::round(v_dist.x);
//	diagram_line_resolution.y = v_dist.y;// std::round(v_dist.y);
//}

void Window::autoSize() {
	draw_object_vec_mutex.lock();
	if (!draw_object_vec.empty()) {
		wykobi::rectangle<float> outer_rect;
		outer_rect = draw_object_vec.front()->getBoundingRectangle();
		for (auto it = draw_object_vec.begin() + 1; it != draw_object_vec.end(); ++it) {
			wykobi::rectangle<float> inner_rect = (*it)->getBoundingRectangle();
			if (inner_rect[0].x < outer_rect[0].x) {
				outer_rect[0].x = inner_rect[0].x;
			}
			if (inner_rect[0].y < outer_rect[0].y) {
				outer_rect[0].y = inner_rect[0].y;
			}
			if (inner_rect[1].x > outer_rect[1].x) {
				outer_rect[1].x = inner_rect[1].x;
			}
			if (inner_rect[1].y > outer_rect[1].y) {
				outer_rect[1].y = inner_rect[1].y;
			}
		}
		wykobi::point2d<float> centre = wykobi::centroid(outer_rect);
		wykobi::vector2d<float> size;
		if (!lock_world_view_scale_button.getState()) {
			size = outer_rect[1] - outer_rect[0];
		}
		else {
			size = outer_rect[1] - outer_rect[0];
			if (size.x > size.y) {
				float ratio = diagram_area.height / diagram_area.width;
				size.y = size.x * world_view.getViewport().height;
			}
			else {
				float ratio = diagram_area.width / diagram_area.height;
				size.x = size.y * ratio;
			}
		}
		world_view.setCenter({ centre.x, centre.y });
		world_view.setSize({ size.x, size.y });

	}
	draw_object_vec_mutex.unlock();
}

void Window::loadShapeFromFile() {
	FileDialog::OpenFile dialog;
	dialog.create();
	if (dialog.getStatus() == FileDialog::Success) {
		loadShapeFromFile(dialog.getPath());
	}
	else if (dialog.getStatus() == FileDialog::Closed) {
		return;
	}
	else {
		std::cout << "Error: FileDialog failed\n";
		return;
	}
}

void Window::loadShapeFromFile(std::string path) {
	std::fstream file;
	file.open(path);

	std::string line;

	std::unordered_map<std::string, std::string> settings_map;

	while (std::getline(file, line)) {
		auto vec_1 = splitString(line, ' ');
		for (std::string & str : vec_1) {
			auto vec_2 = splitString(str, '=');
			if (vec_2.size() == 2) {
				settings_map.emplace(vec_2[0], vec_2[1]);
			}
		}

		for (auto it = settings_map.begin(); it != settings_map.end(); ++it) {
			std::cout << it->first << " -> " << it->second << "\n";
		}

		std::cout << "\n";

	}

	

	file.close();
}

void Window::saveShapeToFile() {
	FileDialog::SaveFile dialog;
	dialog.create();
	if (dialog.getStatus() == FileDialog::Success) {
		saveShapeToFile(dialog.getPath());
	}
	else if (dialog.getStatus() == FileDialog::Closed) {
		return;
	}
	else {
		std::cout << "Error: FileDialog failed\n";
		return;
	}
}
void Window::saveShapeToFile(std::string path) {
	draw_object_vec_mutex.lock();
	std::ofstream file;
	file.open(path);
	for (auto it = draw_object_vec.begin(); it != draw_object_vec.end(); ++it) {
		file << (*it)->toString();
		file << '\n';
	}
	draw_object_vec_mutex.unlock();
	file.flush();
	file.close();
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
				v.position = sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(seg[i].x, seg[i].y), world_view));
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
				v.position = sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(seg[i].x, seg[i].y), world_view));
				diagram_vertex_array.append(v);
			}
		}
		y += diagram_line_resolution.y;
	}

	//text
	window.setView(screen_view);
	window.draw(diagram_vertex_array);
	window.setView(screen_view);
	for (auto & seg : vertical_segments) {
		std::size_t index = (seg[0].y < seg[1].y) ? 0 : 1;
		sf::Text t;
		t.setFillColor(diagram_text_color);
		t.setPosition(sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(seg[index].x, seg[index].y), world_view)));
		t.setPosition({ t.getPosition().x, t.getPosition().y - 20 });	 //HACK
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
		t.setPosition({ t.getPosition().x - 20, t.getPosition().y });	//HACK
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

	if (show_draw_object_button.getState()) {
		//render object names
		window.setView(screen_view);
		draw_object_vec_mutex.lock();
		for (auto it = draw_object_vec.begin(); it != draw_object_vec.end(); ++it) {
			if (!(*it)->name.empty()) {
				sf::Text t;
				t.setFont(*text_font);
				t.setString((*it)->name);
				t.setCharacterSize(draw_object_text_size);
				t.setFillColor(contrastColor((*it)->fill_color));
				setTextPositionCentre(t, sf::Vector2f(window.mapCoordsToPixel((*it)->getCentroid(), world_view)));
				window.draw(t);
			}
		}
		draw_object_vec_mutex.unlock();
	}
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

void Window::setWindowSize(int w, int h) {
	window_mutex.lock();
	window_size = { static_cast<unsigned int>(w), static_cast<unsigned int>(h) };
	update_frame = true;
	window_mutex.unlock();
}

void Window::setDiagramPosition(float x, float y) {
	window_mutex.lock();
	world_view.setCenter(x + world_view.getSize().x / 2, y + world_view.getSize().y / 2);
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

sf::Vector2u Window::getWindowSize() {
	return window_size;
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

sf::Vector2f PolygonShape::getCentroid() {
	auto centre = wykobi::centroid(polygon);
	return { centre.x, centre.y };
}

wykobi::rectangle<float> PolygonShape::getBoundingRectangle() {
	return GeometryDisplay::getBoundingRectangle(polygon);
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

void DrawObject::create(std::unordered_map<std::string, std::string> & settings_map) {
	std::unordered_map<std::string, std::string>::iterator it;
	it = settings_map.find("name");
	if (it != settings_map.end()) {
		name = it->second;
	}
	it = settings_map.find("outer_line");
	if (it != settings_map.end()) {
		std::istringstream(it->second) >> std::boolalpha >> outer_line;
	}
	it = settings_map.find("line_color");
	if (it != settings_map.end()) {
		line_color = parseColor(it->second);
	}
	it = settings_map.find("outer_line_thickness");
	if (it != settings_map.end()) {
		outer_line_thickness = std::atof(it->second.c_str());
	}
	it = settings_map.find("inner_fill");
	if (it != settings_map.end()) {
		std::istringstream(it->second) >> std::boolalpha >> inner_fill;
	}
	it = settings_map.find("fill_color");
	if (it != settings_map.end()) {
		fill_color = parseColor(it->second);
	}
}

std::string DrawObject::toString() {
	std::ostringstream stream;
	if (!name.empty()) {
		stream << "name=\"" << name << "\"" << " ";
	}
	if (outer_line) {
		stream << "outer_line=" << outer_line << " ";
		if (line_color.a != 0xff) {
			stream << "line_color=" << std::hex << line_color.toInteger() << std::hex << line_color.a << " ";
		}
		else {
			stream << "line_color=" << std::hex << line_color.toInteger() << " ";
		}
		stream << "outer_line_thickness=" << outer_line_thickness << " ";
	}
	if (inner_fill) {
		stream << "inner_fill=" << inner_fill << " ";
		if (fill_color.a != 0xff) {
			stream << "fill_color=" << std::hex << fill_color.toInteger() << std::hex << fill_color.a << " ";
		}
		else {
			stream << "fill_color=" << std::hex << fill_color.toInteger() << " ";
		}
	}
	return stream.str();
}

void PolygonShape::create(std::unordered_map<std::string, std::string> & settings_map) {
	std::unordered_map<std::string, std::string>::iterator it;
	it = settings_map.find("polygon");



}

std::string PolygonShape::toString() {
	std::ostringstream stream;
	stream << "type=" << "line" << " ";
	stream << DrawObject::toString();
	stream << "polygon={";
	for (std::size_t i = 0; i < polygon.size(); ++i) {
		stream << "(" << polygon[i].x << "," << polygon[i].y << ")";
	}
	stream << "}";
	stream << " ";
	return stream.str();
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

std::string LineShape::toString() {
	std::ostringstream stream;
	stream << "type=" << "line" << " ";
	stream << DrawObject::toString();
	stream << "thickness=" << thickness << " ";
	stream << "segment={";
	for (std::size_t i = 0; i < segment.size(); ++i) {
		stream << "(" << segment[i].x << "," << segment[i].y << ")";
	}
	stream << "}";
	stream << " ";
	return stream.str();
}

sf::Vector2f LineShape::getCentroid() {
	auto mid = wykobi::segment_mid_point(segment);
	return { mid.x, mid.x };
}

wykobi::rectangle<float> LineShape::getBoundingRectangle() {
	return wykobi::make_rectangle(segment[0], segment[1]);
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

float GeometryDisplay::normalize(float value, float min, float max) {
	return (value - min) / (max - min);
}
float GeometryDisplay::deNormalize(float value, float min, float max) {
	return (value * (max - min) + min);
}

sf::Color GeometryDisplay::contrastColor(sf::Color color) {
	return (color.toInteger() > sf::Uint32(0xffffff) / 2) ? sf::Color::Black : sf::Color::White;
}

void GeometryDisplay::setTextPositionCentre(sf::Text & t, sf::Vector2f point) {
	sf::FloatRect rect = t.getGlobalBounds();
	t.setPosition({ point.x - rect.width / 2.f, point.y - rect.height / 2.f });
}

void GeometryDisplay::setViewPositionCorner(sf::View & view, sf::Vector2f point, int corner) {
	sf::Vector2f vec;
	switch (corner) {
	case 0:
		vec = { view.getSize().x / 2.f, view.getSize().y / 2.f };
		view.setCenter(point + vec);
		break;
	case 1:
		vec = { -(view.getSize().x / 2.f), view.getSize().y / 2.f };
		view.setCenter(point + vec);
		break;
	case 2:
		vec = { -(view.getSize().x / 2.f), -(view.getSize().y / 2.f) };
		view.setCenter(point + vec);
		break;
	case 3:
		vec = { view.getSize().x / 2.f, -(view.getSize().y / 2.f) };
		view.setCenter(point + vec);
		break;
	default:
		break;
	}
}

std::vector<std::string> GeometryDisplay::splitString(std::string & str, char c) {
	std::vector<std::string> str_vec;
	std::size_t i = 0;
	std::size_t j;
	std::size_t len;
	while (i < str.size()) {
		j = str.find_first_of(c, i);
		if (j == str.npos) {
			len = str.size() - i;
			if (len > 0) {
				str_vec.push_back(str.substr(i, str.npos));
			}
			break;
		}
		else {
			len = j - i;
			if (len > 0) {
				str_vec.push_back(str.substr(i, len));
			}
			i = j + 1;
		}
	}
	return str_vec;
}

sf::Color GeometryDisplay::parseColor(std::string & str) {
	if (str.size == 6) {
		std::stringstream ss;
		sf::Uint32 rgb;
		ss << std::hex << str;
		ss >> rgb;
		return sf::Color(rgb);
	}
	else if (str.size() == 8) {
		std::stringstream ss;
		sf::Uint32 rgb;
		sf::Uint8 alpha;
		ss << std::hex << str.substr(0, 6);
		ss >> rgb;
		ss.clear();
		ss << std::hex << str.substr(6, str.npos);
		ss >> alpha;
		sf::Color color(rgb);
		color.a = alpha;
		return color;
	}
	else {
		return sf::Color();
	}
}

wykobi::point2d<float> GeometryDisplay::parsePoint(std::string str) {
	if (str.front() == '(') {
		str.erase(str.begin());
	}
	if (str.back() == ')') {
		str.erase(str.end() - 1);
	}
	auto vec = splitString(str, ',');
	return wykobi::make_point<float>(std::atof(vec[0].c_str()), std::atof(vec[1].c_str()));
}

std::vector<wykobi::point2d<float>> GeometryDisplay::parsePoints(std::string & str) {
	std::vector<wykobi::point2d<float>> out_vec;

	str.erase(str.begin());
	str.erase(str.end() - 1);

	auto str_vec = splitString()

}

//end