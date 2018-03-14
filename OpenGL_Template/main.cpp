#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <math.h>
#include <memory>

#include "loadShader.hpp"
#include "loadBMP_custom.h"
#include "Vertex.h"
#include "ShapeData.h"
#include "ShapeGenerator.h"
#include "Camera.h"
#include "Buffer.h"
#include "VAO.h"
#include "ShapeContainer.h"
#include "Utilities.h"
#include "GL_Scene.h"

#include "GL_Type_Traits.h"
#include "GL_Tuple_Introspect.h"


#define GLM_ENABLE_EXPERIMENTAL
#define DEBUG

struct ApplicationState {
	double time				= 0.0;
	double freqMultiplier	= 0.0;

	SDL_Window*		st_window = nullptr;
	SDL_GLContext	st_opengl = nullptr;

	ApplicationState() {
		if (st_window) SDL_DestroyWindow(st_window);
		if (st_opengl) SDL_GL_DeleteContext(st_opengl);
	}
};



struct opengl_attr_pair
{
	SDL_GLattr key;
	int value;
};

opengl_attr_pair st_config [] =
{
    {SDL_GL_RED_SIZE,					8},
    {SDL_GL_GREEN_SIZE,					8},
    {SDL_GL_BLUE_SIZE,					8},
    {SDL_GL_ALPHA_SIZE,					8},
    {SDL_GL_BUFFER_SIZE,				32},
    {SDL_GL_DOUBLEBUFFER,				1},
    {SDL_GL_DEPTH_SIZE,					24},
    {SDL_GL_STENCIL_SIZE,				8},
    {SDL_GL_ACCUM_RED_SIZE,				0},
    {SDL_GL_ACCUM_GREEN_SIZE,			0},
    {SDL_GL_ACCUM_BLUE_SIZE,			0},
    {SDL_GL_ACCUM_ALPHA_SIZE,			0},
    {SDL_GL_STEREO,						0},
    {SDL_GL_MULTISAMPLEBUFFERS,			1},
    {SDL_GL_MULTISAMPLESAMPLES,			4},
    {SDL_GL_ACCELERATED_VISUAL,			1},
    {SDL_GL_CONTEXT_MAJOR_VERSION,		4},
    {SDL_GL_CONTEXT_MINOR_VERSION,		4},
    {SDL_GL_CONTEXT_EGL,				0},
    {SDL_GL_CONTEXT_FLAGS,				SDL_GL_CONTEXT_DEBUG_FLAG},
    {SDL_GL_CONTEXT_PROFILE_MASK,		SDL_GL_CONTEXT_PROFILE_CORE},
    {SDL_GL_SHARE_WITH_CURRENT_CONTEXT,	1},
    {SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,	1}
};

// // Calling custom debug // //
// // https://bcmpinc.wordpress.com/2015/08/21/debugging-since-opengl-4-3/ // //
// // More info at above website // //
static void __stdcall openglCallbackFunction(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam
) {
	(void)source; (void)type; (void)id; (void)severity; (void)length; (void)userParam;
	fprintf(stderr, "%s\n", message);
	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		fprintf(stderr, "Aborting...\n");
		abort();
	}
}


//// -----INIT----- ////
void initWindow(ApplicationState& _State, GL_Scene& _Scene)
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
	GLuint width	= 1280u;
	GLuint height	= 720u;
	_State.st_window = SDL_CreateWindow("Tutorial 04 - A Cloured Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	assert(_State.st_window != nullptr);
	_State.st_opengl = SDL_GL_CreateContext(_State.st_window);
	assert(_State.st_opengl != nullptr);

	SDL_GL_SetSwapInterval(1);

	// High precision clock interval
	_State.freqMultiplier = 1.0 / SDL_GetPerformanceFrequency();

	// Initial time in clock ticks
	_State.time = _State.freqMultiplier * SDL_GetPerformanceCounter();

	// // Initialise GLEW // //
	glewExperimental = true;
	auto loc_glewok = glewInit();
	assert(loc_glewok == GLEW_OK);

	_Scene.init(width, height);
}

void finish_frame (ApplicationState& _State)
{
	SDL_GL_SwapWindow (_State.st_window);
}


void exit(ApplicationState &_State, GL_Scene& _Scene){
	glUseProgram(0);
	glDeleteProgram(_Scene.m_program_id);
}

bool poll_events (ApplicationState& _State, GL_Scene& _Scene)
{
	SDL_Event loc_event;
	static bool mouseDown = false;
	static bool altDown = false;
	while (SDL_PollEvent (&loc_event)) 
	{
		
		if (loc_event.type == SDL_QUIT) 
		{
			return false;
		}
		if (loc_event.type == SDL_KEYUP) {
			if (loc_event.key.keysym.scancode == SDL_SCANCODE_F) 
			{
				glm::mat4 wldBuffer = glm::mat4(1.0f);
				_Scene.m_cam.focus(wldBuffer);
			}
		}
	}
	return true;
}

void update_camera(ApplicationState& _State, GL_Scene& _Scene) {
	static const auto cMoveSpeed   = glm::vec3(0.02f, 0.01f, 0.1f);
	static const auto cRotateSpeed = glm::vec2(0.01f, 0.01f);

	auto const keyboardState = SDL_GetKeyboardState(nullptr);
	auto mouseDelta  = glm::ivec2();
	auto mouseButton = SDL_GetRelativeMouseState(&mouseDelta.x, &mouseDelta.y);
	auto axisDelta	 = glm::vec3();
	auto rotateDelta = glm::vec2();

	if  (mouseButton & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		rotateDelta = (glm::vec2)mouseDelta;
	}
	if (mouseButton & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
		axisDelta.x = -(float)mouseDelta.x;
		axisDelta.y = (float)mouseDelta.y;
	}
	if (mouseButton & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		axisDelta.z = (float)mouseDelta.y;

	}
	_Scene.m_cam.moveRel(axisDelta * cMoveSpeed);
	_Scene.m_cam.rotateRel(rotateDelta * cRotateSpeed);
}


int main(int, char**)
{
	GL_Scene			_Scene;
	ApplicationState	_State;

	initWindow(	_State, _Scene);
	while (poll_events (_State, _Scene))
	{
		update_camera(_State, _Scene);
		_Scene.render_frame();
		finish_frame (_State);
	}

	exit(_State, _Scene);
	return 0;
}
