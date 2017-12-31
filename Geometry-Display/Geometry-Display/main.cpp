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

#include "FileDialog.hpp"

int main() {
	std::shared_ptr<sf::Font> arial(new sf::Font());
	if (!arial->loadFromFile("fonts/arial.ttf")) {
		std::cout << "Font load failed\n";
		return EXIT_FAILURE;
	}

	GeometryDisplay::Window w;
	
	w.create({ 1000, 600 });
	w.setMouseMove(true);

	GeometryDisplay::LineShape line(wykobi::make_segment<float>(10, 10, 300, 400));
	line.thickness = 10.f;
	w.addShape(line);

	GeometryDisplay::PolygonShape p1(wykobi::make_polygon(wykobi::make_circle(100.f, 100.f, 50.f), 100));
	p1.name = "P1";
	p1.inner_fill = true;
	p1.outer_line = true;
	p1.fill_color = sf::Color::Green;
	p1.line_color = sf::Color::Cyan;

	GeometryDisplay::PolygonShape p2(wykobi::make_polygon(wykobi::make_circle(100.f, -500.f, 50.f), 100));
	p2.name = "P2";
	p2.inner_fill = true;
	p2.outer_line = true;
	p2.fill_color = sf::Color::Green;
	p2.line_color = sf::Color::Cyan;

	w.addShape(p1);
	w.addShape(p2);

	w.saveShapeToFile();

	w.join();
	

	return EXIT_SUCCESS;
}


//end