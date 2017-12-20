//Author: Sivert Andresen Cubedo
#pragma once

#ifndef GeometryDisplay_HEADER
#define GeometryDisplay_HEADER

#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <memory>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>
#include <wykobi_algorithm.hpp>

namespace GeometryDisplay {
	
	class DrawObject {
	public:
		std::string name;
		sf::Color color = sf::Color::White;
		bool outer_line = false;
		virtual void appendVertex(sf::VertexArray & vertex_arr) = 0;
		virtual DrawObject* clone() = 0;
	};
	class TriangleShape : public DrawObject {
	public:
		wykobi::triangle<float, 2> triangle;
		TriangleShape(float x0, float y0, float x1, float y1, float x2, float y2);
		TriangleShape(std::vector<sf::Vector2f> & vec);
	};
	class PolygonShape : public DrawObject {
	public:
		wykobi::polygon<float, 2> polygon;
		PolygonShape(wykobi::polygon<float, 2> poly);
		PolygonShape* clone() override;
		void appendVertex(sf::VertexArray & vertex_arr) override;
	};
	class LineShape : public DrawObject {
	public:
		wykobi::segment<float, 2> segment;
		float thickness = 1.f;
		LineShape(wykobi::segment<float, 2> seg);
		LineShape* clone() override;
		void appendVertex(sf::VertexArray & vertex_arr) override;
	};

	class Window {
	private:
		sf::RenderWindow window;

		std::atomic<int> update_interval = 50;		//in ms
		std::atomic<int> window_width = 500;		//in px
		std::atomic<int> window_height = 500;		//in px
		std::atomic<bool> update_settings = false;
		std::atomic<bool> update_frame = false;
		std::atomic<bool> running = true;

		std::mutex shape_vec_mutex;
		std::vector<std::unique_ptr<DrawObject>> shape_vec;
		sf::VertexArray shape_vertex_array = sf::VertexArray(sf::Triangles);
		
		sf::VertexArray ui_vertex_array = sf::VertexArray(sf::Triangles);
		std::vector<sf::Text> sf_text_vec;

		std::thread window_thread;

		/*
		Window thread function
		*/
		void windowHandler();

		/*
		Render UI
		*/
		void renderUI();

		/*
		Renders and displays next frame
		Must be called from window_thread
		*/
		void renderFrame();
		
	public:
		//Window();							//constructor

		void addShape(DrawObject & shape);
		void addShape(wykobi::polygon<float, 2> poly);
		void addShape(wykobi::segment<float, 2> seg);
		//std::vector<Shape*> addShape(std::vector<wykobi::polygon<float, 2>> & vec);
		void clearShapeVec();

		/*
		Set update interval
		In milliseconds
		*/
		void setUpdateInterval(int t);

		/*
		Set window size
		*/
		void setSize(int w, int h);

		/*
		Get window width
		*/
		int getWindowWidth();

		/*
		Get window height
		*/
		int getWindowHeight();

		/*
		Wait for window_thread to close
		This will block if the thread is joinable, else nothing happens
		*/
		void join();

		/*
		Init display
		*/
		void create();										

		/*
		Close display
		Kill thread
		*/
		void close();						
	};

}

#endif // !GeometryDisplay_HEADER


//end
