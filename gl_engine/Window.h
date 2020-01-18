#ifndef GL_ENGINE_WINDOW_H
#define GL_ENGINE_WINDOW_H
#define DEBUG

#include <cstdlib>
#include <vector>
#include <cassert>
#include <csignal>
#include <string>

#include <SDL.h>
#include <GL/glew.h>

namespace gl_engine
{
	/*
	
		Basic window type used for displaying the rendered output to the screen
	
	*/
	struct opengl_attr_pair
	{
		SDL_GLattr key;
		int value;
	};


	struct Window
	{
		// // ----- CONSTANTS ----- // //
		static constexpr float DEFAULT_WINDOWS_DPI = 96.0f;

		// // ----- CONSTRUCTORS ----- // //
		Window(std::string name, std::vector<opengl_attr_pair> st_config, GLuint width, GLuint height);

		// // ----- INIT ----- // //
		void init(std::vector<opengl_attr_pair> st_config, GLuint width, GLuint height);

		// // ----- GENERAL METHODS ----- // //
		void append_title(std::string str);
		void finish_frame();
	private:
		void print_gl_properties();

		// // ----- GETTERS ----- // //
	public:
		GLint width();
		GLint height();
	private:
		void init_dimensions(GLuint* width, GLuint* height);


		// // ----- MEMBER VARIABLES ----- // //
	public:
		SDL_Window *	m_st_window = nullptr;
		SDL_GLContext	m_st_opengl = nullptr;
	private:
		std::string m_name = "";
	};
} // namespace gl_engine
#endif