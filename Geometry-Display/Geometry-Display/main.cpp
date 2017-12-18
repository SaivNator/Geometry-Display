//Author: Sivert Andresen Cubedo

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

#include <SFML\Graphics.hpp>
#include <wykobi.hpp>

#include "GeometryDisplay.hpp"


//void testThread(std::mutex* print_mutex) {
//	std::thread::id id = std::this_thread::get_id();
//
//	print_mutex->lock();
//	std::cout << ">>>" << id << "<<<: " << "Staring...\n";
//	print_mutex->unlock();
//
//	for (std::size_t i = 0; i < 10; ++i) {
//		print_mutex->lock();
//		std::cout << ">>>" << id << "<<<: " << i << "\n";
//		print_mutex->unlock();
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//	}
//	print_mutex->lock();
//	std::cout << ">>>" << id << "<<<: " << "Ending...\n";
//	print_mutex->unlock();
//}



int main() {
	
	//GeometryDisplay::Window display;

	//display.init();

	std::this_thread::sleep_for(std::chrono::seconds(10));

	std::cout << ">>>" << std::this_thread::get_id() << "<<<: " << "main()\n";

	//display.close();


	return EXIT_SUCCESS;
}


//end