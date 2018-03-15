#include "Window.h"

void Window::init(std::vector<opengl_attr_pair> st_config, GLuint width, GLuint height)
{
#ifdef _DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
#endif
	SDL_Init(SDL_INIT_EVERYTHING);
	std::atexit(SDL_Quit);

	for (const auto& it : st_config)
	{
		SDL_GL_SetAttribute(it.key, it.value);
	}

	// // Request a debug context // //
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
	);

	// // Create window // //
	m_st_window = SDL_CreateWindow("Tutorial 04 - A Cloured Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	assert(m_st_window != nullptr);
	m_st_opengl = SDL_GL_CreateContext(m_st_window);
	assert(m_st_opengl != nullptr);

	SDL_GL_SetSwapInterval(1);

	// High precision clock interval
	//.freqMultiplier = 1.0 / SDL_GetPerformanceFrequency();

	// Initial time in clock ticks
	//_State.time = _State.freqMultiplier * SDL_GetPerformanceCounter();

	// // Initialise GLEW // //
	glewExperimental = true;
	auto loc_glewok = glewInit();
	assert(loc_glewok == GLEW_OK);
}

void Window::finish_frame()
{
	SDL_GL_SwapWindow(m_st_window);
}