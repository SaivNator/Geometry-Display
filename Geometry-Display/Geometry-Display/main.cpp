//Author: Sivert Andresen Cubedo

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

#include <SFML\Graphics.hpp>
#include <wykobi.hpp>

#include "GeometryDisplay.hpp"

int main() {
	
	GeometryDisplay::Window display;

	display.create();

	GeometryDisplay::PolygonShape circle(wykobi::make_polygon<float>(wykobi::make_circle<float>(100, 100, 50), 10));

	display.addShape(circle);

	

	display.join();
	display.close();

	return EXIT_SUCCESS;
}


//end