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
#include "Window.h"
#include "GL_Scene.h"
#include "Timer.h"

#include "GL_Type_Traits.h"
#include "GL_Tuple_Introspect.h"


#define GLM_ENABLE_EXPERIMENTAL
#define DEBUG

//struct ApplicationState {
//	double time				= 0.0;
//	double freqMultiplier	= 0.0;
//};

std::vector<opengl_attr_pair> st_config =
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

int main(int, char**)
{
	//!!!To do: refactor timer
	Window				_Window;
	GL_Scene			_Scene;
	Timer				_Timer;

	GLuint width	= 1280u;
	GLuint height	= 720u;

	_Window.init(st_config, width, height);
	_Scene.init(width, height);
	_Timer.init();

	while (_Scene.pollEvents())
	{
		_Scene.m_cam.update();
		_Timer.update();
		_Scene.renderFrame(_Timer);
		_Window.finish_frame();
	}
	return 0;
}
