//Author: Sivert Andresen Cubedo
#pragma once

#ifndef GeometryDisplay_HEADER
#define GeometryDisplay_HEADER

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <memory>
#include <cmath>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>
#include <wykobi_algorithm.hpp>

#include "StandardCursor.hpp"

namespace GeometryDisplay {
	
	class DrawObject {
	public:
		std::string name;
		sf::Color fill_color;
		sf::Color line_color;
		bool outer_line = false;
		bool inner_fill = true;
		float outer_line_thickness = 2.f;
		virtual void appendVertex(sf::VertexArray & vertex_arr) = 0;
		virtual DrawObject* clone() = 0;
	};
	class TriangleShape : public DrawObject {
	public:
		wykobi::triangle<float, 2> triangle;
		TriangleShape(float x0, float y0, float x1, float y1, float x2, float y2);
		TriangleShape* clone() override;
		void appendVertex(sf::VertexArray & vertex_arr) override;
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

	class ToggleButton : public sf::Drawable {
	private:
		sf::IntRect area;
		bool toggle = false;
		bool bounce = false;

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;

	public:
		std::string not_toggle_text = "button off";
		std::string toggle_text = "button on";
		sf::Color not_toggle_text_color;
		sf::Color toggle_text_color;
		sf::Color not_toggle_color;
		sf::Color toggle_color;
		unsigned int text_char_size = 10;

		ToggleButton(sf::IntRect button_area);

		/*
		Check if mouse_pos is inseide area, if true then toggle
		*/
		void click(const sf::Vector2i mouse_pos);

		/*
		Reset bounce of button
		*/
		void release();

		/*
		Get button state
		false = not toggeled
		true = toggeled
		*/
		bool getState();

		void appendVertex(sf::VertexArray & vertex_arr);
	};

	class Window {
	private:
		sf::RenderWindow window;

		std::string window_title = "Geometry Display";

		std::shared_ptr<sf::Font> text_font;

		int update_interval = 50;				//in ms
		unsigned int window_width = 500;		//in px
		unsigned int window_height = 500;		//in px
		bool update_frame = false;
		bool running = true;

		std::mutex window_mutex;
		std::thread window_thread;

		sf::Color window_background_color = sf::Color::White;

		std::mutex draw_object_vec_mutex;
		std::vector<std::unique_ptr<DrawObject>> draw_object_vec;
		sf::VertexArray draw_object_vertex_array = sf::VertexArray(sf::Triangles);
		std::vector<sf::Text> shape_text_vec;
		
		sf::VertexArray ui_vertex_array = sf::VertexArray(sf::Triangles);
		std::vector<sf::Text> ui_text_vector;
		float ui_border_thickness = 50.f;
		sf::Color ui_border_color = sf::Color(129, 129, 129);

		sf::VertexArray diagram_vertex_array = sf::VertexArray(sf::Lines);
		std::vector<sf::Text> diagram_text_vector;
		sf::Color diagram_text_color = sf::Color::Black;
		unsigned int diagram_text_char_size = 10;

		sf::View screen_view;
		sf::View world_view;	
		sf::FloatRect diagram_area;
		wykobi::vector2d<float> diagram_line_resolution = wykobi::make_vector<float>(50.f, 50.f);
		float diagram_line_thickness = 2.f;
		sf::Color diagram_line_color = sf::Color::Blue;

		//mouse move
		bool mouse_move = false;
		bool mouse_left_down = false;
		bool mouse_left_bounce = false;
		sf::Vector2i mouse_pos;
		sf::Vector2f mouse_current_pos;
		sf::Vector2f mouse_start_pos;

		//mouse zoom
		bool mouse_zoom = false;
		float mouse_zoom_amount = 1.1f;
		bool mouse_middle_down = false;

		/*
		Window thread function
		*/
		void windowHandler();

		/*
		Update view
		*/
		void updateView();

		/*
		Render diagram
		*/
		void renderLines();

		/*
		Render UI
		*/
		void renderUI();

		/*
		Renders and displays next frame
		Must be called from window_thread
		*/
		void renderDrawObject();
		
	public:
		Window();
		Window(std::shared_ptr<sf::Font> font_ptr);							//constructor

		void addShape(DrawObject & shape);
		void addShape(wykobi::polygon<float, 2> poly);
		void addShape(wykobi::segment<float, 2> seg);
		//std::vector<Shape*> addShape(std::vector<wykobi::polygon<float, 2>> & vec);
		void clearShapeVec();

		/*
		Set mouse move
		*/
		void setMouseMove(bool v);

		/*
		Set mouse zoom
		*/
		void setMouseZoom(bool v);

		/*
		Set diagram rotation
		*/
		void rotateDiagram(float r);

		/*
		Set diagram resolution
		*/
		void setDiagramLineResolution(float x, float y);

		/*
		Set diagram position
		*/
		void setDiagramPosition(float x, float y);

		/*
		Zoom diagram
		*/
		void zoomDiagram(float scalar);

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
		Set title
		*/
		void setTitle(std::string title);

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


	/*
	Make two triangles representing a line with thickness
	*/
	std::vector<wykobi::triangle<float, 2>> makeTriangleLine(float x0, float y0, float x1, float y1, float thickness);
	std::vector<wykobi::triangle<float, 2>> makeTriangleLine(wykobi::segment<float, 2> & seg, float thickness);

	/*
	Floor point to closest resolution
	*/
	float getClosestPointInRes(float v, float res);

	/*
	Get smallest bounding rectangle
	*/
	wykobi::rectangle<float> getBoundingRectangle(wykobi::polygon<float, 2> & poly);

	/*
	Check if segment is intersectiong polygon
	*/
	bool segmentIntersectPolygon(wykobi::segment<float, 2> & seg, wykobi::polygon<float, 2> & poly);

	/*
	Zoom view at pixel
	*/
	void zoomViewAtPixel(sf::Vector2i pixel, sf::View & view, sf::RenderWindow & window, float zoom);
}

#endif // !GeometryDisplay_HEADER


//end
