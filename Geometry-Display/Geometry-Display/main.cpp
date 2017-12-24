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
	line.thickness = 10.f;
	std::vector<GeometryDisplay::Window> win_vec(1);
	std::size_t i = 0;
	for (auto & w : win_vec) {
		w.create();
		w.setDiagramPosition(0, 100);
		w.setDiagramOriginCorner(i);

		w.setMouseMove(true);
		w.setUpdateInterval(10);

		//w.rotateDiagram(90.f);

		std::ostringstream title;
		title << "Window ";
		title << i;
		w.setTitle(title.str());
		w.addShape(line);
		++i;
	}




	for (auto & w : win_vec) {
		w.join();
	}

	return EXIT_SUCCESS;
}


//end