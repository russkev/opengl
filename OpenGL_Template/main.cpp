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
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#define DEBUG
//#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

static constexpr auto POSITION_ATTR = 0u;
static constexpr auto COLOR_ATTR    = 1u;
static constexpr auto MODEL_ATTR    = 2u;

struct ApplicationState {
	GLuint programID       = 0;
	GLuint matrixID        = 0;
	GLuint darkenID        = 0;

	GLuint TheBufferID        = 0;
	GLuint CubeVertexArrayID  = 0;
	GLuint ArrowVertexArrayID = 0;
	GLuint TheVertexArrayID   = 0;
	GLuint ColorBufferID      = 0;
	std::vector<GLuint> VertexArrays;
	//GLuint TheBufferID      = 0;
	GLuint MatrixBufferID     = 0;

	GLuint numBuffers      = 1;
	GLuint numInstances    = 0;
	//GLuint numIndices      = 0;
	GLuint cubeNumIndices  = 0;
	GLuint arrowNumIndices = 0;
	//GLuint matrixLoc	   = 0;
	GLsizeiptr sizeOfArrow = 0;
	GLsizeiptr sizeOfCube  = 0;

	GLsizeiptr sizeOfArrowVerts = 0;
	GLsizeiptr sizeOfCubeVerts = 0;

	glm::mat4 view        = glm::mat4();
	glm::mat4 projection  = glm::mat4();
	std::vector<GLfloat> offsets;
	Camera cam;

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
	_State.view       = _State.cam.getWorldToViewMatrix();
	//_State.view       = glm::lookAt(glm::vec3(4, 4, 3), glm::vec3(-1, -1, 0), glm::vec3(0, 1, 0));

	static ShapeData g_buffer_data_triangle = ShapeGenerator::makeTriangle();
	static ShapeData g_buffer_data_cube     = ShapeGenerator::makeCube();
	static ShapeData g_buffer_data_arrow    = ShapeGenerator::makeArrow();
	_State.arrowNumIndices = g_buffer_data_arrow.numIndices();
	_State.cubeNumIndices  = g_buffer_data_cube.numIndices();

	glm::vec3 arrowTranslate = { 30.0f, 38.0f, 0.0f };
	for (auto& i : g_buffer_data_arrow.vertices) {
		i.position += arrowTranslate;
	}

	glm::vec3 cubeTranslate = { 30.0f, 30.0f, 0.0f };
	for (auto& i : g_buffer_data_cube.vertices) {
		i.position += cubeTranslate;
	}


	// // TEST // //
	loadBMP_custom BMP1 ("uvtemplate.bmp");
	// // END TEST // //


	for (auto& it : g_buffer_data_arrow.indices) {
		it += ((GLushort)g_buffer_data_cube.vertices.size());
	}

	// // Push cube and arrow vertices to :graphics card memory (location: TheBufferID):
	glGenBuffers(1, &_State.TheBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.TheBufferID);
	glBufferData(GL_ARRAY_BUFFER, 
		g_buffer_data_cube.sizeVertices() + g_buffer_data_cube.sizeIndices() + 
		g_buffer_data_arrow.sizeVertices() + g_buffer_data_arrow.sizeIndices(), nullptr, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	// //             Target buffer    Offset        Size of the data being replaced            Pointer to the data
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, g_buffer_data_cube.sizeVertices(), &g_buffer_data_cube.vertices.front());		//CUBE VERTS
	currentOffset += g_buffer_data_cube.sizeVertices(); //336 (4*6*14)																 
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, g_buffer_data_cube.sizeIndices(),  &g_buffer_data_cube.indices.front());		//CUBE INDICES
	currentOffset += g_buffer_data_cube.sizeIndices();  //480 (336 + 72*2)
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, g_buffer_data_arrow.sizeVertices(),  &g_buffer_data_arrow.vertices.front());	//ARROW VERTS
	currentOffset += g_buffer_data_arrow.sizeVertices();  //672																		  
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, g_buffer_data_arrow.sizeIndices(),   &g_buffer_data_arrow.indices.front());		//ARROW INDICES


	const Vertex* base = nullptr;
	_State.sizeOfArrow      = g_buffer_data_arrow.sizeVertices() + g_buffer_data_arrow.sizeIndices();
	_State.sizeOfArrowVerts = g_buffer_data_arrow.sizeVertices();
	_State.sizeOfCube       = g_buffer_data_cube.sizeVertices() + g_buffer_data_cube.sizeIndices();
	_State.sizeOfCubeVerts  = g_buffer_data_cube.sizeVertices();
	// // Generate the Vertex Aray Object (VAO)
	// // This will later store all the information about what is actually in the vertex buffer

	// // CUBE // //
	glGenVertexArrays(1,		   &_State.CubeVertexArrayID);       // Create a VAO ID
	_State.VertexArrays.push_back(	_State.CubeVertexArrayID);
	glBindVertexArray(				_State.CubeVertexArrayID);		// Attach the Vertex Array reader to the VAO ID
	glEnableVertexAttribArray(POSITION_ATTR);
	glVertexAttribPointer(    POSITION_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), &base->position);
	glEnableVertexAttribArray(COLOR_ATTR);
	glVertexAttribPointer(    COLOR_ATTR,    3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), &base->color);

	// // ARROW // // 
	glGenVertexArrays(1,			&_State.ArrowVertexArrayID);
	_State.VertexArrays.push_back(   _State.ArrowVertexArrayID);
	glBindVertexArray(				 _State.ArrowVertexArrayID);
	glEnableVertexAttribArray(POSITION_ATTR);
	glVertexAttribPointer(    POSITION_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)_State.sizeOfCube);
	glEnableVertexAttribArray(COLOR_ATTR);
	glVertexAttribPointer(    COLOR_ATTR,    3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(_State.sizeOfCube + sizeof(glm::tvec3<GLfloat>)));


	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _State.TheBufferID);

	


	// // Push offset information to the graphics card memory
	// // Make a line of evenly spaced cubes from line start to line finish
	GLfloat lineStart = -8.0f;
	GLfloat lineEnd   = +4.0f;
	_State.numInstances   = 10;
	GLfloat spacing = (lineEnd - lineStart) / _State.numInstances;
	while (lineStart <= lineEnd) {
		_State.offsets.push_back(lineStart);
		lineStart += spacing;
	}
	
	glGenBuffers(1, &_State.MatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.MatrixBufferID);
	
	// Initial matrix
	std::vector<glm::mat4> MVP;
	for (GLuint i = 0; i < _State.numInstances; ++i) {
		MVP.push_back(glm::mat4(1.0));
	}
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*MVP.size(), MVP.data(), GL_DYNAMIC_DRAW);
	
	for (auto& j : _State.VertexArrays) {
		glBindVertexArray(j);
		for (int i = 0; i < 4; ++i) {
			//glEnableVertexArrayAttrib(_State.ArrowVertexArrayID, _State.matrixLoc + i);
			glEnableVertexAttribArray(	MODEL_ATTR + i);
			glVertexAttribPointer(		MODEL_ATTR + i, 4u, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * (i * 4)));
			glVertexAttribDivisor(		MODEL_ATTR + i, 1);
		}
	}

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
	 const auto tpi = 0;// glm::pi<float>()*float(_State.time);

	 //Colour multiplier based on time and a cosine wave
	 GLfloat cmAmplitude = 0.5f;
	 GLfloat cmFreq      = 0.1f;
	 GLfloat cmOffset    = 0.5f;
	 GLfloat colMult     = cmAmplitude * cos(2.0f * glm::pi<float>() * cmFreq * GLfloat(_State.time)) + cmOffset;

	 //Send multiplier to the currently bound shader
	 glUniform1f(_State.darkenID, colMult);


	 // Matrix transformations
	 std::vector<glm::mat4> MVP;
	 for (GLuint i = 0; i < _State.numInstances; i++) {
		 MVP.push_back(glm::scale(glm::rotate(glm::translate(_State.projection*_State.cam.getWorldToViewMatrix(), 
			 glm::vec3(0.0f, 0.0f, _State.offsets.at(i))),	//Translate
			 0.6f*tpi, glm::vec3(0.0f, 1.0f, 1.0f)),		//Rotate
			 glm::vec3(0.1f, 0.1f, 0.1f)));					//Scale
	 }

	// // Cube // // 
	glBindVertexArray(_State.CubeVertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.MatrixBufferID);
	auto matrixBufferPtr = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	std::copy(MVP.begin(), MVP.end(), matrixBufferPtr);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _State.TheBufferID);
	glDrawElementsInstanced(GL_TRIANGLES, _State.cubeNumIndices, GL_UNSIGNED_SHORT, (void*)_State.sizeOfCubeVerts, GLsizei(_State.numInstances));


	// // ARROW // //
	glBindVertexArray(_State.ArrowVertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.MatrixBufferID);
	matrixBufferPtr = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	std::copy(MVP.begin(), MVP.end(), matrixBufferPtr);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _State.TheBufferID);
	glDrawElementsInstanced(GL_TRIANGLES, _State.arrowNumIndices,  GL_UNSIGNED_SHORT, (void*)(_State.sizeOfCube+_State.sizeOfArrowVerts), GLsizei(_State.numInstances));
}


void exit(ApplicationState &_State) {
	glDeleteBuffers(_State.numBuffers, &_State.TheBufferID);
	glDeleteVertexArrays(_State.numBuffers, &_State.TheVertexArrayID);
	glDeleteVertexArrays(_State.numBuffers, &_State.ArrowVertexArrayID);
	glDeleteVertexArrays(_State.numBuffers, &_State.CubeVertexArrayID);
}

bool poll_events (ApplicationState& _State)
{
	SDL_Event loc_event;
	static bool mouseDown = false;
	static bool altDown = false;
	while (SDL_PollEvent (&loc_event)) {
		
		if (loc_event.type == SDL_QUIT) {
			return false;
		}
		if (loc_event.type == SDL_MOUSEBUTTONDOWN){
			mouseDown = true;
		}
		if (loc_event.type == SDL_MOUSEBUTTONUP) {
			mouseDown = false;
		}
		if (loc_event.type == SDL_KEYDOWN) {
			if (loc_event.key.keysym.scancode == SDL_SCANCODE_LALT) {
				altDown = true;
			}
			_State.cam.positionUpdate(loc_event.key.keysym.scancode);
		}
		if (loc_event.type == SDL_KEYUP) {
			if (loc_event.key.keysym.scancode == SDL_SCANCODE_LALT) {
				altDown = false;
			}
		}
		if (loc_event.type == SDL_MOUSEMOTION) {
			_State.cam.mouseUpdate(glm::vec2(loc_event.motion.x, loc_event.motion.y), altDown, mouseDown);
			break;
		}
		if (loc_event.type == SDL_MOUSEWHEEL) {
			_State.cam.scrollUpdate((float)loc_event.wheel.y);
			break;
		}
	}
	return true;
}


int main(int, char**)
{
	ApplicationState _State;
	init(_State);
	while (poll_events (_State))
	{
		render_frame (_State);
		finish_frame (_State);
	}

	exit(_State);
	return 0;
}
