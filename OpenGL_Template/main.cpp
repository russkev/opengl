#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>


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

#define GLM_ENABLE_EXPERIMENTAL
#define DEBUG
//#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct ApplicationState {
	GLuint programID       = 0;
	GLuint matrixID        = 0;
	GLuint darkenID        = 0;

	GLuint VertexBufferID  = 0;
	GLuint VertexArrayID   = 0;
	GLuint ColorBufferID   = 0;
	GLuint IndexBufferID   = 0;
	GLuint MatrixBufferID  = 0;

	GLuint numBuffers      = 1;
	GLuint numIndices      = 0;
	GLuint matrixLoc	   = 0;

	glm::mat4 view        = glm::mat4();
	glm::mat4 projection  = glm::mat4();
	std::vector<GLfloat> offsets;

	double time = 0.0;
	double freqMultiplier = 0.0;

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
void init (ApplicationState& _State)
{
#ifdef _DEBUG
	SDL_LogSetAllPriority (SDL_LOG_PRIORITY_VERBOSE);
#endif
	SDL_Init (SDL_INIT_EVERYTHING);
	std::atexit(SDL_Quit);

	for(const auto& it : st_config)
	{
		SDL_GL_SetAttribute(it.key, it.value);
	}

	// // Request a debug context // //
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
	);

	// // Create window // //
	auto width = 1280u, height = 720u;
	_State.st_window = SDL_CreateWindow ("Tutorial 04 - A Cloured Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	assert (_State.st_window != nullptr);
	_State.st_opengl = SDL_GL_CreateContext (_State.st_window);
	assert (_State.st_opengl != nullptr);

	// High precision clock interval
	_State.freqMultiplier = 1.0 / SDL_GetPerformanceFrequency();

	// Initial time in clock ticks
	_State.time = _State.freqMultiplier * SDL_GetPerformanceCounter();

	// // Initialise GLEW // //
	glewExperimental = true;
	auto loc_glewok = glewInit ();
	assert (loc_glewok == GLEW_OK);

	// // Check OpenGL properties // //
	printf("OpenGL loaded\n");
	//gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

#ifdef DEBUG
	// // Enable the debug callback // //
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE
	);
#endif // DEBUG

	// // Dark blue background // //
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// // Enable depth test // //
	glEnable(GL_DEPTH_TEST);
	// // Accept fragment shader if it closer to the camera than the previous one
	glDepthFunc(GL_LESS);

	// // Create and compile our GLSL program from the shaders // //
	_State.programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");

	// // Get handle for out "MVP" uniform. MVP = Model View Projection // //
	// // Only during initialization // //
	_State.matrixID = glGetUniformLocation(_State.programID, "MVP");
	_State.darkenID = glGetUniformLocation(_State.programID, "darken");

	_State.projection = glm::perspective(glm::radians(50.0f), float(width) / float(height), 0.1f, 100.0f);
	_State.view       = glm::lookAt(glm::vec3(4, 4, 3), glm::vec3(-1, -1, 0), glm::vec3(0, 1, 0));

	static ShapeData g_buffer_data_triangle = ShapeGenerator::makeTriangle();
	static ShapeData g_buffer_data_cube     = ShapeGenerator::makeCube();
	_State.numIndices = g_buffer_data_cube.numIndeces();

	// // TEST // //
	loadBMP_custom BMP1 ("uvtemplate.bmp");
	// // END TEST // //

	// // Push cube vertices graphics card memory (location: VertexBufferID):
	glGenBuffers(1, &_State.VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, g_buffer_data_cube.sizeVertices(), &g_buffer_data_cube.vertices.front(), GL_STATIC_DRAW);

	// // Push cube indeces to graphics card memory (location: IndexBufferID):
	glGenBuffers(1, &_State.IndexBufferID);                                          // Create a bufferID
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _State.IndexBufferID);                     // Attach it to the Element Array buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_buffer_data_cube.sizeIndeces(), &g_buffer_data_cube.indeces.front(), GL_STATIC_DRAW); // Move the data into the buffer

	// // Generate the Vertex Aray Object (VAO)
	// // This will later store all the information about the what is actually in the vertex buffer
	glGenVertexArrays(1, &_State.VertexArrayID);    // Create a VAO ID
	glBindVertexArray(_State.VertexArrayID);		// Attach the Vertex Array reader to the VAO ID

	const Vertex* base = nullptr;
	// // Open the VAO in order for it to be written to
	glEnableVertexArrayAttrib(_State.VertexArrayID, 0);
	// // Store information about the buffer using the latest GL_ARRAY_BUFFER to be called.
	glVertexAttribPointer(
		0,							// // attribute 0, must match the layout in shader // //
		3u,							// // size (1,2,3 or 4) // //
		GL_FLOAT,					// // type // //
		GL_FALSE,					// // normalised  // //
		sizeof(Vertex),				// // stride // //
		&base->position				// // array buffer offset // //
	);
	// Both colour and vertex information are held in the same buffer
	// So that doesn't need to be done again for the colour.
	glEnableVertexArrayAttrib(_State.VertexArrayID, 1);
	glVertexAttribPointer(1, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), &base->color);


	// // Push offset information to the graphics card memory
	_State.offsets = { -8.0f, -5.0f, -3.0f, 0.0f, 2.5f, 4.0f };
	glGenBuffers(1, &_State.MatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.MatrixBufferID);

	return;
}

void finish_frame (ApplicationState& _State)
{
	SDL_GL_SwapWindow (_State.st_window);
}

void render_frame (ApplicationState& _State)
{
	// // Tutorial from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ // //
	// // Clear the screen // //
	// // This clears both the colour buffer and the depth buffer at the same time
	// // 
	 glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	 glUseProgram(_State.programID);

	 //Get time
	 _State.time    = _State.freqMultiplier * SDL_GetPerformanceCounter();
	 const auto tpi = glm::pi<float>()*float(_State.time);

	 //Colour multiplier based on time and a cosine wave
	 GLfloat cmAmplitude = 0.5f;
	 GLfloat cmFreq      = 0.1f;
	 GLfloat cmOffset    = 0.5f;
	 GLfloat colMult     = cmAmplitude * cos(2.0f * glm::pi<float>() * cmFreq * GLfloat(_State.time)) + cmOffset;

	 //Send multiplier to the currently bound shader
	 glUniform1f(_State.darkenID, colMult);

	 // Matrix transformations
	 std::vector<glm::mat4> MVP;
	 for (int i = 0; i < _State.offsets.size(); i++) {
		 MVP.push_back(glm::scale(glm::rotate(glm::translate(_State.projection*_State.view, 
			 glm::vec3(0.0f, 0.0f, _State.offsets.at(i))),	//Translate
			 0.6f*tpi, glm::vec3(0.0f, 1.0f, 1.0f)),		//Rotate
			 glm::vec3(0.5f, 0.5f, 0.5f)));					//Scale
	 }
	 glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*MVP.size(), MVP.data(), GL_STATIC_DRAW);
	 _State.matrixLoc = 2;
	 for (int i = 0; i < 4; ++i) {
		 //auto test = sizeof(glm::mat4)*MVP.size();
		 glVertexAttribPointer(_State.matrixLoc + i, 4u, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * (i*4)));
		 glEnableVertexAttribArray(_State.matrixLoc + i);
		 glVertexAttribDivisor(_State.matrixLoc + i, 1);
	 }

	 //Send Matrix to the currently bound shader
	 //glUniformMatrix4fv(_State.matrixID, 1u, GL_FALSE, &MVP[0][0][0]);

	 //Tell OpenGL which array buffer to use for upcoming draw call
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _State.IndexBufferID);

	 //Draw call uses all the relevent OpenGL global variables set up to this point
	 //glDrawElements(GL_TRIANGLES, _State.numIndices, GL_UNSIGNED_SHORT, nullptr);
	 glDrawElementsInstanced(GL_TRIANGLES, _State.numIndices, GL_UNSIGNED_SHORT, nullptr, _State.offsets.size());

}
void exit(ApplicationState &_State) {
	glDeleteBuffers(_State.numBuffers, &_State.VertexBufferID);
	glDeleteBuffers(_State.numBuffers, &_State.ColorBufferID);
	glDeleteBuffers(_State.numBuffers, &_State.IndexBufferID);
	glDeleteVertexArrays(_State.numBuffers, &_State.VertexArrayID);
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
	ApplicationState _State;
	init(_State);
	while (poll_events ())
	{
		render_frame (_State);
		finish_frame (_State);
	}

	exit(_State);
	return 0;
}
