//Author: Sivert Andresen Cubedo
#pragma once

#ifndef Window_HEADER
#define Window_HEADER

#include <iostream>
#include <thread>

#include <SFML\Graphics.hpp>

class Window {
public:
	/*
	Constructor
	*/
	Window(sf::VideoMode mode, const std::string& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

	/*
	Join window thread
	*/
	void join();

	/*
	Terminate window thread
	*/
	void close();

private:
	std::thread m_thread;
	bool m_running;
	std::chrono::milliseconds m_sleep_duration;
	
	/*
	Function for window thread
	*/
	void windowHandler(sf::VideoMode mode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings);

	/*
	Function to handle window events
	*/
	void eventHandler(sf::RenderWindow & window);

	/*
	Function to handle updates
	*/
	void updateHandler(sf::RenderWindow & window, sf::Time & dt);

	/*
	Function to handle draw calls
	*/
	void drawHandler(sf::RenderWindow & window);
};

#endif // !Window_HEADER

