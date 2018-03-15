#pragma once

#include <cstdlib>
#include <vector>
#include <cassert>

#include <SDL.h>
#include <GL/glew.h>

struct opengl_attr_pair
{
	SDL_GLattr key;
	int value;
};


struct Window
{
	void init(std::vector<opengl_attr_pair> st_config, GLuint width, GLuint height);
	void finish_frame();


public:
	SDL_Window *	m_st_window		= nullptr;
	SDL_GLContext	m_st_opengl		= nullptr;
};