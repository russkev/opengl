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
//#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct ApplicationState {
	GLuint programID = 0;
	GLuint matrixID = 0;

	GLuint VertexBufferID = 0;
	GLuint ColorBufferID = 0;
	GLuint IndexBufferID = 0;


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

	// // Enable the debug callback // //
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(
		GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE
	);

	// // Dark blue background // //
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// // Enable depth test // //
	glEnable(GL_DEPTH_TEST);
	// // Accept fragment shader if it closer to the camera than the previous one
	glDepthFunc(GL_LESS);

	// // --- Create the VAO (Vertex Array Object) --- // //
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// // Create and compile our GLSL program from the shaders // //
	GLuint programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");

	// // Use our shader // //
	glUseProgram(programID);

	// // Get handle for out "MVP" uniform. MVP = Model View Projection // //
	// // Only during initialization // //
	_State.matrixID = glGetUniformLocation(programID, "MVP");

	//static ShapeData g_buffer_data_triangle = ShapeGenerator::makeTriangle();
	//static ShapeData g_buffer_data_cube     = ShapeGenerator::makeCube();


	GLfloat verts[] =
	{
		+0.0f, +0.0f,
		+1.0f, +1.0f,
		-1.0f, +1.0f,
		-1.0f, -1.0f,
		+1.0f, -1.0f,
	};

	static GLushort indeces[] = {
		0,1,2, 0,3,4
	};

	// // TEST // //

	loadBMP_custom BMP1 ("uvtemplate.bmp");
	// // END TEST // //

	// // Generate one buffer, put the resulting identifier in vertex buffer // //
	// // The following commands will talk about our 'vertexbuffer' buffer // //
	// // Give our vertices to OpenGL
	// // All this code needs to be in blocks of the three lines

	//glGenBuffers(1, &VertexBuffer[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[1]);
	//glBufferData(GL_ARRAY_BUFFER, g_buffer_data_triangle.sizeVertices(), &g_buffer_data_triangle.vertices.front(), GL_STATIC_DRAW);

	// // Push triangle vertices:
	glGenBuffers(1, &_State.VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// // Rig up vertex array:
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


	// // Push triangle indeces:
	glGenBuffers(1, &_State.IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _State.IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);






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

	 

	  // Array of three vectors which represent the three vertices // //
	 //static const glm::vec3 g_vertex_buffer_data_cube[] = {
		// glm::vec3(-1.0f,-1.0f,-1.0f),
		// glm::vec3(-1.0f,-1.0f, 1.0f),
		// glm::vec3(-1.0f, 1.0f, 1.0f),
		// glm::vec3(1.0f,  1.0f,-1.0f),
		// glm::vec3(-1.0f,-1.0f,-1.0f),
		// glm::vec3(-1.0f, 1.0f,-1.0f),
		// glm::vec3(1.0f, -1.0f, 1.0f),
		// glm::vec3(-1.0f,-1.0f,-1.0f),
		// glm::vec3(1.0f, -1.0f,-1.0f),
		// glm::vec3(1.0f,  1.0f,-1.0f),
		// glm::vec3(1.0f, -1.0f,-1.0f),
		// glm::vec3(-1.0f,-1.0f,-1.0f),
		// glm::vec3(-1.0f,-1.0f,-1.0f),
		// glm::vec3(-1.0f, 1.0f, 1.0f),
		// glm::vec3(-1.0f, 1.0f,-1.0f),
		// glm::vec3(1.0f,-1.0f,  1.0f),
		// glm::vec3(-1.0f,-1.0f, 1.0f),
		// glm::vec3(-1.0f,-1.0f,-1.0f),
		// glm::vec3(-1.0f, 1.0f, 1.0f),
		// glm::vec3(-1.0f,-1.0f, 1.0f),
		// glm::vec3(1.0f,-1.0f,  1.0f),
		// glm::vec3(1.0f, 1.0f,  1.0f),
		// glm::vec3(1.0f,-1.0f, -1.0f),
		// glm::vec3(1.0f, 1.0f, -1.0f),
		// glm::vec3(1.0f,-1.0f, -1.0f),
		// glm::vec3(1.0f, 1.0f,  1.0f),
		// glm::vec3(1.0f,-1.0f,  1.0f),
		// glm::vec3(1.0f, 1.0f,  1.0f),
		// glm::vec3(1.0f, 1.0f, -1.0f),
		// glm::vec3(-1.0f, 1.0f,-1.0f),
		// glm::vec3(1.0f, 1.0f,  1.0f),
		// glm::vec3(-1.0f, 1.0f,-1.0f),
		// glm::vec3(-1.0f, 1.0f, 1.0f),
		// glm::vec3(1.0f, 1.0f,  1.0f),
		// glm::vec3(-1.0f, 1.0f, 1.0f),
		// glm::vec3(1.0f,-1.0f,  1.0f)
	 //};

	 _State.time = _State.freqMultiplier * SDL_GetPerformanceCounter();
	
	 //static glm::tvec3<GLfloat> g_color_buffer_data_cube[12 * 3 * 3];

	 // // Change colours of cube over time // //
	 //for (int i = 0; i < 12 * 3 * 3; ++i) {
		// for (int j = 0; j < 3; ++j) {
		//	 if (g_vertex_buffer_data_cube[i][j] == 1) {
		//		 if (fmod(GLfloat(time), 2.0f) < 1) {
		//			 g_color_buffer_data_cube[i][j] = fmod(GLfloat(time), 1.0f);
		//		 }
		//		 else {
		//			 g_color_buffer_data_cube[i][j] = 2 - fmod(GLfloat(time), 2.0f);
		//		 }
		//	 }
		//	 else {
		//		 g_color_buffer_data_cube[i][j] = 0;
		//	 }
		//}
	 //};

	 //glGenBuffers(1, &VertexBuffer[0]);
	 //glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[0]);
	 //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_cube), g_vertex_buffer_data_cube, GL_STATIC_DRAW);

	 //glGenBuffers(1, &ColorBuffer[0]);
	 //glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer[0]);
	 //glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data_cube), g_color_buffer_data_cube, GL_STATIC_DRAW);

	 ////int temp = SDL_GetWindowSurface(_State.st_window)->w;
	 //// // Projection matrix : 45 degree Field of View, display range : 0.1 <-> 100 units // //
	 //glm::mat4 Projection = glm::perspective(glm::radians(30.0f), float(SDL_GetWindowSurface(_State.st_window)->w) / SDL_GetWindowSurface(_State.st_window)->h, 0.1f, 100.0f);
	 //// // Orthographic projection // //
	 ////glm::mat4 Projection = glm::ortho(-2.0f, 2.0f, -2.0f, 1.556f, 0.1f, 100.0f);

	 //// // Camera Matrix // //
	 //glm::mat4 View = glm::lookAt(
		// glm::vec3(4, 4, 3),
		// glm::vec3(0, 0, 0),
		// glm::vec3(0, 1, 0)
	 //);

	 //// // Model matrix : an identity matrix (wil be at the origin) // //
	 //glm::vec3 modelPosition(0.0f, 0.0f, -2.5f);
	 //glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), modelPosition);
	 //glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	 //glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	 ////glm::mat4 Model = modelScale * modelTranslate;
	 //glm::mat4 Model_cube = modelTranslate * modelRotate * modelScale;
	 //glm::mat4 Model_triangle = glm::mat4(1.0f);


	 //// // Send our transformation matrix to the currently bound shader, in the "MVP" uniform
	 //// // This is done in the main loop since each model will have a different MVP matrix (at least for the M part)
	 //glm::mat4 rotationOffset = glm::rotate(glm::mat4(1.0f), glm::radians(float(time*100)), glm::vec3(0.0f, 1.0f, 1.0f));
	 //glm::mat4 MVP = Projection*View*Model_cube*rotationOffset;
	 ////glUniformMatrix4fv(_State.matrixID, 1, GL_FALSE, &MVP[0][0]);

	 //// // Enable the appropriate attributes in the vertex shader
	 ////glEnableVertexAttribArray(0);
	 ////glEnableVertexAttribArray(1);


	 //// // 1st attribute buffer : vertices // //
	 ////glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[0]);
	 ////glVertexAttribPointer(
	 ////	0,			// // attribute 0, could be any number but must match the layout in shader // //
	 ////	3,			// // size // //
	 ////	GL_FLOAT,	// // type // //
	 ////	GL_FALSE,	// // normalised  // //
	 ////	0,			// // stride // //
	 ////	(void*)0	// // array buffer offset // //
	 ////	);

	 //// // 2nd attribute buffer : colours // //
	 //
	 ////glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer[0]);
	 ////glVertexAttribPointer(
		//// 1,
		//// 3,
		//// GL_FLOAT,
		//// GL_FALSE,
		//// 0,
		//// (void*)0
	 ////);
	 //// // Draw the triangle! // //
	 ////glDrawArrays(GL_TRIANGLES, 0, 12*3);
 
	 //MVP = Projection*View*rotationOffset;
	 //glUniformMatrix4fv(_State.matrixID, 1, GL_FALSE, &MVP[0][0]);
	 //// // 1st attribute buffer : vertices // //
	 ////glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[1]);
	 //glVertexAttribPointer(
		// 0,			
		// 3,			
		// GL_FLOAT,	
		// GL_FALSE,	
		// sizeof(Vertex),			
		// (void*)0	
	 //);
	 // // 2nd attribute buffer : colours // //
	 //glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[1]);
	 //glVertexAttribPointer(
		// 1,
		// 3,
		// GL_FLOAT,
		// GL_FALSE,
		// sizeof(Vertex),
		// (char*)(sizeof(glm::vec3))
	 //);
	 //glDrawArrays(GL_TRIANGLES, 0, 3*12);
	

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);

	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
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

	return 0;
}
