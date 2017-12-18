//Author: Sivert Andresen Cubedo
#pragma once

#ifndef GeometryDisplay_HEADER
#define GeometryDisplay_HEADER

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>

namespace GeometryDisplay {
	
	struct Shape {
		std::vector<sf::Vertex> verticies;
		virtual std::vector<sf::Vertex> & getVerticies() = 0;
	};
	
	class Window {
	private:

		sf::RenderWindow window;

		std::condition_variable window_thread_cv;
		bool window_thread_running = true;
		bool window_thread_update_waiting = false;
		std::thread window_thread;
		std::mutex window_thread_mutex;

		std::vector<Shape> shape_vec;

		sf::VertexArray triangle_vertex_array_vec;

		sf::VertexArray border_frame_vertex_array;

		void inputLoopFunc();				//for window_thread
		
	public:
		//Window();							//constructor

		void appendShapeVec(Shape & shape);
		void appendShapeVec(std::vector<Shape> & vec);
		void clearShapeVec();


		void init();						//start thread
		void show();						//show window
		void hide();						//hide window
		void close();						//clone thread and free memory
	};

	struct TriangleShape : public Shape {
		TriangleShape(float x0, float y0, float x1, float y1, float x2, float y2);
		TriangleShape(std::vector<sf::Vector2f> & vec);
	};
	struct PolygonShape : public Shape {
		PolygonShape(std::vector<sf::Vector2f> & vec);
		PolygonShape(std::vector<wykobi::point2d<float>> & vec);
	};

}

#endif // !GeometryDisplay_HEADER


//end
