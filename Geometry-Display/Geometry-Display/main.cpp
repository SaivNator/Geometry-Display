//Author: Sivert Andresen Cubedo

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

#include <SFML\Graphics.hpp>
#include <wykobi.hpp>

#include "GeometryDisplay.hpp"
#include "TextShape.hpp"

int main() {
	sf::Font arial;
	if (!arial.loadFromFile("fonts/arial.ttf")) {
		std::cout << "Font load failed\n";
		return EXIT_FAILURE;
	}

	GeometryDisplay::Window display;

	display.create();

	GeometryDisplay::LineShape line(wykobi::make_segment<float>(10, 10, 300, 400));


	line.thickness = 10.f;

	display.addShape(line);

	display.setDiagramOriginCorner(1);
	display.setDiagramRotaton(-30.f);





	//display.addShape(wykobi::make_polygon<float>(wykobi::make_circle<float>(100, 100, 50), 10));
	//
	//wykobi::segment<float, 2> seg = wykobi::make_segment(10.f, 10.f, 321.f, 100.f);
	//
	//display.addShape(seg);
	

	display.join();
	display.close();

	return EXIT_SUCCESS;
}


//end