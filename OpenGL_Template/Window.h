#pragma once
#include <SDL.h>

struct Window
{
	void init();

public:
	SDL_Window * m_st_window		= nullptr;
	SDL_GLContext	m_st_opengl		= nullptr;
};