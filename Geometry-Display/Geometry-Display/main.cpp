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

	

	display.join();
	display.close();

	return EXIT_SUCCESS;
}


//end