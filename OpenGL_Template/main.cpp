#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "loadShader.hpp"
//#include "glad/glad.h"

static SDL_Window*		st_window = nullptr;
static SDL_GLContext	st_opengl = nullptr;

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
static void APIENTRY openglCallbackFunction(
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



GLuint init () 
{
#ifdef _DEBUG
	SDL_LogSetAllPriority (SDL_LOG_PRIORITY_VERBOSE);
#endif
	SDL_Init (SDL_INIT_EVERYTHING);
	std::atexit([] ()
	{
		if (st_window) SDL_DestroyWindow(st_window);
		if (st_opengl) SDL_GL_DeleteContext(st_opengl);
		SDL_Quit();
	});

	for(const auto& it : st_config)
	{
		SDL_GL_SetAttribute(it.key, it.value);
	}

	// // Request a debug context // //
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
	);

	st_window = SDL_CreateWindow (nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280u, 720u, SDL_WINDOW_OPENGL);
	assert (st_window != nullptr);
	st_opengl = SDL_GL_CreateContext (st_window);
	assert (st_opengl != nullptr);

	// // Check OpenGL properties // //
	printf("OpenGL loaded\n");
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	// // Enable the debug callback // //
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
	);

	glewExperimental = true;
	auto loc_glewok = glewInit ();
	assert (loc_glewok == GLEW_OK);

	// // --- Create the VAO (Vertex Array Object) --- // //
	GLuint VertexArray1D;
	glGenVertexArrays(1, &VertexArray1D);
	glBindVertexArray(VertexArray1D);

	// // Array of three vectors which represent the three vertices // //
	static const GLfloat g_vertex_buffer_data[] = {
		0.0f,   1.0f,  0.0f,
		-1.0f,  -1.0f,  0.0f,
		1.0f,  -1.0f,  0.0f,
	};

	// // Identify vertex buffer // //
	GLuint VertexBuffer;
	// // Generate one buffer, put the resulting identifier in vertex buffer // //
	glGenBuffers(1, &VertexBuffer);
	// // The following commands will talk about our 'vertexbuffer' buffer // //
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	// // Give our vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// // Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");



	return programID;
}

void finish_frame () 
{
	SDL_GL_SwapWindow (st_window);
}

void render_frame (const GLuint& programID) 
{
	//// // First attribut buffer: vertices // //
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	//glVertexAttribPointer(
	//	0,			// // attribute 0, could be any number but must match the layout in shader // //
	//	3,			// // size // //
	//	GL_FLOAT,	// // type // //
	//	GL_FALSE,	// // normalised  // //
	//	0,			// // stride // //
	//	(void*)0	// // array buffer offset // //
	//	);
	//
	//// // Draw the triangle! // //
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDisableVertexAttribArray(0);


	// // Tutorial from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ // //
	 glClearColor (1.0f, 0.0f, 0.0f, 1.0f);
	 glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	 glUseProgram(programID);
}

bool poll_events () 
{
	SDL_Event loc_event;
	while (SDL_PollEvent (&loc_event)) {
		if (loc_event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

int main(int, char**)
{
	GLuint programID = init ();

	while (poll_events ())
	{
		render_frame (programID);	
		finish_frame ();
	}

	return 0;
}
