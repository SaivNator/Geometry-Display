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

	GeometryDisplay::LineShape line(wykobi::make_segment<float>(10, 10, 300, 400));
	line.thickness = 10.f;
	std::vector<GeometryDisplay::Window> win_vec(4);
	int i = 0;
	for (auto & w : win_vec) {
		w.create();
		w.setDiagramPosition(-100, 100);
		w.setDiagramOriginCorner(i++);
		w.setDiagramRotaton(0.f);
		w.addShape(line);
	}




	for (auto & w : win_vec) {
		w.join();
	}

	return EXIT_SUCCESS;
}


//end