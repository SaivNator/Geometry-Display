//Author: Sivert Andresen Cubedo

#include <iostream>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>

#include "FileDialog.hpp"

int main() {

	std::cout << "HELLO\n";

	FileDialog::OpenFile d;

	d.create();

	d.getPath();


	return EXIT_SUCCESS;
}