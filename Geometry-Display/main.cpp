//Author: Sivert Andresen Cubedo

#include <iostream>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>

#include "FileDialog.hpp"

#include "UIRectangle.hpp"

int main() {

	GUI::UIRectangle rect(100, 100, 100, 100);

	std::cout << "HELLO\n";

	FileDialog::OpenFile d;

	d.create();

	d.getPath();


	return EXIT_SUCCESS;
}