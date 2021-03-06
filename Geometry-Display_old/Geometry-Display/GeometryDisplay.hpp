//Author: Sivert Andresen Cubedo
#pragma once

#ifndef GeometryDisplay_HEADER
#define GeometryDisplay_HEADER

#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <memory>
#include <functional>

#include <cmath>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>
#include <wykobi_algorithm.hpp>

#include "StandardCursor.hpp"
#include "FileDialog.hpp"

namespace GeometryDisplay {
	class DrawObject {
	public:
		std::string name;
		bool inner_fill = true;
		bool outer_line = false;
		sf::Color fill_color;
		sf::Color line_color;
		float outer_line_thickness = 2.f;

		DrawObject() = default;
		DrawObject(std::unordered_map<std::string, std::string> & settings_map);
		virtual sf::Vector2f getCentroid() = 0;
		virtual wykobi::rectangle<float> getBoundingRectangle() = 0;
		virtual DrawObject* clone() = 0;
		virtual void appendVertex(sf::VertexArray & vertex_arr) = 0;
		virtual std::string toString();
	};
	class PolygonShape : public DrawObject {
	public:
		wykobi::polygon<float, 2> polygon;
		PolygonShape(wykobi::polygon<float, 2> poly);
		PolygonShape(std::unordered_map<std::string, std::string> & settings_map);
		PolygonShape* clone() override;
		sf::Vector2f getCentroid() override;
		wykobi::rectangle<float> getBoundingRectangle() override;
		void appendVertex(sf::VertexArray & vertex_arr) override;
		std::string toString() override;
	};
	class LineShape : public DrawObject {
	public:
		wykobi::segment<float, 2> segment;
		float thickness = 1.f;
		LineShape(wykobi::segment<float, 2> seg);
		LineShape(std::unordered_map<std::string, std::string> & settings_map);
		LineShape* clone() override;
		sf::Vector2f getCentroid() override;
		wykobi::rectangle<float> getBoundingRectangle() override;
		void appendVertex(sf::VertexArray & vertex_arr) override;
		std::string toString() override;
	};

	class UIPosition {
	protected:
		sf::IntRect area;
	public:
		void positionOver(UIPosition & parent);
		void positionUnder(UIPosition & parent);
		void positionLeft(UIPosition & parent);
		void positionRight(UIPosition & parent);
		sf::Vector2i getSize();
		sf::Vector2i getPosition();
		sf::IntRect getArea();
	};

	class Button : public sf::Drawable, public UIPosition {
	protected:
		std::shared_ptr<sf::Font> text_font;
	public:
		/*
		Click
		*/
		virtual void click(const sf::Vector2i & mouse_pos) = 0;

		/*
		Release
		*/
		virtual void release() = 0;

		/*
		Set Button area
		*/
		void setArea(sf::IntRect button_area);

		/*
		Set Button font
		*/
		void setFont(std::shared_ptr<sf::Font> ptr);
	};

	class PushButton : public Button {
	private:
		bool is_clicked = false;
		bool bounce = false;
		bool force_push = false;
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	public:

		std::function<void()> push_function;

		std::string not_click_text;
		std::string click_text;
		sf::Color not_click_text_color;
		sf::Color click_text_color;
		sf::Color not_click_color;
		sf::Color click_color;
		unsigned int text_char_size = 10;

		/*
		Click button
		*/
		void click(const sf::Vector2i & mouse_pos) override;

		/*
		Reset button when mouse button is released
		*/
		void release() override;
	};

	class ToggleButton : public Button {
	private:
		bool toggle = false;
		bool bounce = false;
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	public:
		std::function<void(bool)> toggle_function;

		std::string not_toggle_text;
		std::string toggle_text;
		sf::Color not_toggle_text_color;
		sf::Color toggle_text_color;
		sf::Color not_toggle_color;
		sf::Color toggle_color;
		unsigned int text_char_size = 10;

		/*
		Override button, set toggle
		*/
		void setToggle(bool v);

		/*
		Check if mouse_pos is inside area, if true then toggle
		*/
		void click(const sf::Vector2i & mouse_pos) override;

		/*
		Reset bounce of button
		*/
		void release() override;
	};

	class PolygonShapeMaker : public sf::Drawable {
	private:
		wykobi::polygon<float, 2> m_polygon;

		bool m_finished = false;

		const sf::View & m_screen_view;
		const sf::View & m_world_view;

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	public:

		sf::Color m_draw_point_color = sf::Color::Red;
		sf::Color m_draw_line_color = sf::Color::Black;

		/*
		Constructor
		*/
		PolygonShapeMaker(sf::View & screen_view, sf::View & world_view);

		/*
		Add vertex to Shape
		return:
			true if point is legal
			false if point is illegal
		*/
		bool addPoint(sf::Vector2f point);
		
		/*
		Finish polygon
		return:
			true if finish is legal
			false if finish is illegal
		*/
		bool finishPolygon();

		/*
		Retrive PolygonShape and reset PolygonShapeMaker
		*/
		PolygonShape retrivePolygonShape();
	};
	class LineShapeMaker : public sf::Drawable {
		std::unique_ptr<LineShape> line_shape;
	private:

	public:

	};

	class Window {
	private:
		sf::RenderWindow window;

		std::string window_title = "Geometry Display";

		std::shared_ptr<sf::Font> text_font;

		int update_interval = 16;				//in ms
		sf::Vector2u window_size = { 500, 500 };
		bool update_frame = false;
		bool running = true;

		std::mutex window_mutex;
		std::thread window_thread;

		sf::Color window_background_color = sf::Color::White;

		std::mutex draw_object_vec_mutex;
		std::vector<std::unique_ptr<DrawObject>> draw_object_vec;
		sf::VertexArray draw_object_vertex_array = sf::VertexArray(sf::Triangles);
		unsigned int draw_object_text_size = 20;
		
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
		//sf::Vector2i line_screen_distance = { 50, 50 };
		sf::Color diagram_line_color = { 0, 0, 255, 255 / 2 };

		//mouse move
		bool mouse_move = true;
		bool mouse_left_down = false;
		bool mouse_left_bounce = false;
		sf::Vector2i mouse_pos;
		sf::Vector2f mouse_current_pos;
		sf::Vector2f mouse_start_pos;
		float mouse_zoom_amount = 1.1f;
		bool mouse_middle_down = false;

		//show draw object name
		bool show_draw_object_name = false;

		//lock scale
		bool lock_world_view_scale = false;

		//make polygon mode
		bool m_make_polygon_mode = false;
		PolygonShapeMaker m_polygon_shape_maker;

		//Buttons
		PushButton clear_draw_object_vec_button;
		PushButton load_draw_object_button;
		PushButton save_draw_object_button;
		ToggleButton show_draw_object_name_button;
		ToggleButton lock_world_view_scale_button;
		ToggleButton mouse_move_button;
		PushButton auto_size_button;
		PushButton make_polygon_button;
		PushButton make_line_button;

		/*
		Button member functions
		*/
		void buttonFunc_clear_draw_object();
		void buttonFunc_load_draw_object();
		void buttonFunc_save_draw_object();
		void buttonFunc_show_draw_object_name(bool t);
		void buttonFunc_lock_world_view_scale(bool t);
		void buttonFunc_mouse_move(bool t);
		void buttonFunc_auto_size();
		void buttonFunc_make_polygon();
		//void buttonFunc_make_line();

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

		/*
		Auto size diagram
		Based on shapes in window
		*/
		void autoSize();

		/*
		Auto line resolution
		*/
		void autoLineResolution();
		
		Window();
	public:
		Window(std::shared_ptr<sf::Font> font_ptr);							//constructor
		Window(std::string font_path);

		/*
		Init display
		*/
		void create();
		void create(sf::Vector2u win_size);

		/*
		Wait for window_thread to close
		This will block if the thread is joinable, else nothing happens
		*/
		void join();

		/*
		Close display
		Kill thread
		*/
		void close();

		/*
		Append shape to window
		*/
		void addShape(DrawObject & shape);					//will clone shape
		void addShape(std::unique_ptr<DrawObject> & ptr);	//will move ptr
		void addShape(wykobi::polygon<float, 2> poly);
		void addShape(wykobi::segment<float, 2> seg);

		/*
		Get screen view
		*/
		sf::View getScreenView();

		/*
		Get world view
		*/
		sf::View getWorldView();

		/*
		Clear shapes from window
		*/
		void clearShapeVec();

		/*
		Set lock screen scale
		*/
		void setLockScreenScale(bool b);

		/*
		Set mouse move
		*/
		void setMouseMove(bool v);

		/*
		Set diagram resolution
		*/
		void setDiagramLineResolution(float x, float y);

		/*
		Set diagram position
		*/
		void setDiagramPosition(float x, float y);

		/*
		Set update interval
		In milliseconds
		*/
		void setUpdateInterval(int t);

		/*
		Set diagram size
		*/
		void setDiagramSize(sf::Vector2f size);

		/*
		Set diagram position
		*/
		void setDiagramPosition(sf::Vector2f pos);

		/*
		Set window size
		*/
		void setWindowSize(int w, int h);

		/*
		Set title
		*/
		void setTitle(std::string title);

		/*
		Get window size
		*/
		sf::Vector2u getWindowSize();

		/*
		Load shapes from file
		(will prompt dialog)
		*/
		void loadShapeFromFile();
		void loadShapeFromFile(std::string path);

		/*
		Save files to file
		(will promt dialog)
		*/
		void saveShapeToFile();
		void saveShapeToFile(std::string path);

					
	};


	/*
	Make two triangles representing a line with thickness
	*/
	std::vector<wykobi::triangle<float, 2>> makeTriangleLine(float x0, float y0, float x1, float y1, float thickness);
	std::vector<wykobi::triangle<float, 2>> makeTriangleLine(wykobi::segment<float, 2> & seg, float thickness);

	/*
	Make triangles representing point with radius
	*/
	std::vector<wykobi::triangle<float, 2>> makeTrianglePoint(float x, float y, float radius, std::size_t point_count);
	std::vector<wykobi::triangle<float, 2>> makeTrianglePoint(wykobi::point2d<float> point, float radius, std::size_t point_count);

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

	/*
	Normalize
	*/
	float normalize(float value, float min, float max);

	/*
	Denormalise
	*/
	float deNormalize(float value, float min, float max);

	/*
	Get contrast color
	Black or White
	*/
	sf::Color contrastColor(sf::Color color);

	/*
	Position sf::Text centre at point
	*/
	void setTextPositionCentre(sf::Text & t, sf::Vector2f point);

	/*
	Position sf::View corner at point
	corner:
		0 = top left
		1 = top right
		2 = bottom right
		4 = bottom left
	*/
	void setViewPositionCorner(sf::View & view, sf::Vector2f point, int corner);

	/*
	Split string with char
	*/
	std::vector<std::string> splitString(std::string & str, char c);

	/*
	Capture strings inside encaptulation
	*/


	/*
	Parse std::string to sf::color
	*/
	sf::Color parseColor(std::string & str);

	/*
	Parse shape verticies from std::string
	*/
	wykobi::point2d<float> parsePoint(std::string str);
	std::vector<wykobi::point2d<float>> parsePoints(std::string & str);

}

#endif // !GeometryDisplay_HEADER


//end
