#ifndef GL_ENGINE_WINDOW_H
#define GL_ENGINE_WINDOW_H

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
		Window(std::string name, std::vector<opengl_attr_pair> st_config, GLuint width, GLuint height);
		void init(std::vector<opengl_attr_pair> st_config, GLuint width, GLuint height);
		void append_title(std::string str);
		void finish_frame();


	public:
		SDL_Window *	m_st_window = nullptr;
		SDL_GLContext	m_st_opengl = nullptr;
	private:
		std::string m_name = "";
	};
} // namespace gl_engine
#endif