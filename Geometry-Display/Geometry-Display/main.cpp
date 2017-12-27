//Author: Sivert Andresen Cubedo

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <memory>

#include <SFML\Graphics.hpp>
#include <wykobi.hpp>

#include "GeometryDisplay.hpp"
#include "TextShape.hpp"

int main() {
	std::shared_ptr<sf::Font> arial(new sf::Font());
	if (!arial->loadFromFile("fonts/arial.ttf")) {
		std::cout << "Font load failed\n";
		return EXIT_FAILURE;
	}

	GeometryDisplay::LineShape line(wykobi::make_segment<float>(10, 10, 300, 400));

	std::vector<GeometryDisplay::PolygonShape> poly_vec;

	poly_vec.push_back(GeometryDisplay::PolygonShape(wykobi::make_polygon(wykobi::make_circle(100.f, 100.f, 50.f), 100)));

	line.thickness = 10.f;
	std::vector<GeometryDisplay::Window> win_vec(1);
	std::size_t i = 0;
	for (auto & w : win_vec) {
		w.create();

		w.setMouseMove(true);
		w.setMouseZoom(true);
		w.setUpdateInterval(10);

		std::ostringstream title;
		title << "Window ";
		title << i;
		w.setTitle(title.str());
		w.addShape(line);

		for (GeometryDisplay::DrawObject & o : poly_vec) {
			o.inner_fill = true;
			o.outer_line = true;
			o.fill_color = sf::Color::Green;
			o.line_color = sf::Color::Cyan;
			w.addShape(o);
		}

		++i;
	}




	for (auto & w : win_vec) {
		w.join();
	}

	return EXIT_SUCCESS;
}


//end