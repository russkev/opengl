#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>


#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "loadShader.hpp"

#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

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



std::vector<glm::mat4> init (GLuint& programID, GLuint& matrixID, std::vector<GLuint>& VertexBuffer, std::vector<GLuint>& ColorBuffer)
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

	// // Create window // //
	auto width = 1280u, height = 720u;
	st_window = SDL_CreateWindow ("Tutorial 04 - A Cloured Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	assert (st_window != nullptr);
	st_opengl = SDL_GL_CreateContext (st_window);
	assert (st_opengl != nullptr);

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
	GLuint VertexArray1D;
	glGenVertexArrays(1, &VertexArray1D);
	glBindVertexArray(VertexArray1D);

	// // Create and compile our GLSL program from the shaders // //
	programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");

	// // Get handle for out "MVP" uniform. MVP = Model View Projection // //
	// // Only during initialization // //
	matrixID = glGetUniformLocation(programID, "MVP");


	// // Projection matrix : 45 degree Field of View, display range : 0.1 <-> 100 units // //
	glm::mat4 Projection = glm::perspective(glm::radians(30.0f), float(width) / float(height), 0.1f, 100.0f);
	// // Orthographic projection // //
	//glm::mat4 Projection = glm::ortho(-2.0f, 2.0f, -2.0f, 1.556f, 0.1f, 100.0f);

	// // Camera Matrix // //
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 4, 3),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	
	// // Model matrix : an identity matrix (wil be at the origin) // //
	glm::vec3 modelPosition(0.0f, 0.0f, -2.5f);
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), modelPosition);
	glm::mat4 modelScale     = glm::scale    (glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 modelRotate	 = glm::rotate   (glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//glm::mat4 Model = modelScale * modelTranslate;
	glm::mat4 Model_cube     = modelTranslate * modelRotate * modelScale;
	glm::mat4 Model_triangle = glm::mat4(1.0f);

	//glm::mat4 Model = glm::mat4(1.0f);

	// // Our ModelViewProjection : multiplication of our three matrices
	std::vector<glm::mat4> MVP;
	MVP.push_back(Projection * View * Model_cube);
	MVP.push_back(Projection * View * Model_triangle);



	// // Array of three vectors which represent the three vertices // //
	static const GLfloat g_vertex_buffer_data_cube[] = {
		-1.0f,-1.0f,-1.0f, 
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 
		1.0f,  1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f, -1.0f,-1.0f,
		1.0f,  1.0f,-1.0f,
		1.0f, -1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	static const GLfloat g_vertex_buffer_data_triangle[] = {
		0.0f, 1.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	// // Array of colours // //
	static const GLfloat g_color_buffer_data_cube[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	static const GLfloat g_color_buffer_data_triangle[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	// // TEST // //

	// // END TEST // //

	// // Generate one buffer, put the resulting identifier in vertex buffer // //
	// // The following commands will talk about our 'vertexbuffer' buffer // //
	// // Give our vertices to OpenGL
	// // All this code needs to be in blocks of the three lines

	glGenBuffers(1, &VertexBuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_cube), g_vertex_buffer_data_cube, GL_STATIC_DRAW);

	glGenBuffers(1, &ColorBuffer[0]);	
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer[0]);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data_cube),  g_color_buffer_data_cube,  GL_STATIC_DRAW);

	glGenBuffers(1, &VertexBuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_triangle), g_vertex_buffer_data_triangle, GL_STATIC_DRAW);

	glGenBuffers(1, &ColorBuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data_triangle), g_color_buffer_data_triangle, GL_STATIC_DRAW);

	return MVP;
}

void finish_frame () 
{
	SDL_GL_SwapWindow (st_window);
}

void render_frame (
	const GLuint& programID, 
	const GLuint& matrixID, 
	const std::vector<GLuint>& VertexBuffer, 
	const std::vector<GLuint>& ColorBuffer, 
	const std::vector<glm::mat4> MVP)
{



	// // Tutorial from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ // //
	// // Clear the screen // //
	 glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	 // // Use our shader // //
	 glUseProgram(programID);

	 // // Send our transformation matrix to the currently bound shader, in the "MVP" uniform
	 // // This is done in the main loop since each model will have a different MVP matrix (at least for the M part)
	 glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0][0]);

	 // // 1st attribute buffer : vertices // //
	 glEnableVertexAttribArray(0);
	 glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[0]);
	 glVertexAttribPointer(
	 	0,			// // attribute 0, could be any number but must match the layout in shader // //
	 	3,			// // size // //
	 	GL_FLOAT,	// // type // //
	 	GL_FALSE,	// // normalised  // //
	 	0,			// // stride // //
	 	(void*)0	// // array buffer offset // //
	 	);

	 // // 2nd attribute buffer : colours // //
	 glEnableVertexAttribArray(1);
	 glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer[0]);
	 glVertexAttribPointer(
		 1,
		 3,
		 GL_FLOAT,
		 GL_FALSE,
		 0,
		 (void*)0
	 );
	 // // Draw the triangle! // //
	 glDrawArrays(GL_TRIANGLES, 0, 12*3);
 

	 glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[1][0][0]);
	 // // 1st attribute buffer : vertices // //
	 glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[1]);
	 glVertexAttribPointer(
		 0,			
		 3,			
		 GL_FLOAT,	
		 GL_FALSE,	
		 0,			
		 (void*)0	
	 );
	 // // 2nd attribute buffer : colours // //
	 glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer[1]);
	 glVertexAttribPointer(
		 1,
		 3,
		 GL_FLOAT,
		 GL_FALSE,
		 0,
		 (void*)0
	 );
	 glDrawArrays(GL_TRIANGLES, 0, 3);

	 glDisableVertexAttribArray(0);
	 glDisableVertexAttribArray(1);
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
	GLuint programID, matrixID;
	std::vector<GLuint> VertexBuffer, ColorBuffer;
	VertexBuffer.resize(2); ColorBuffer.resize(2);
	std::vector<glm::mat4> MVP = init(programID, matrixID, VertexBuffer, ColorBuffer);


	while (poll_events ())
	{
		render_frame (programID, matrixID, VertexBuffer, ColorBuffer, MVP);
		finish_frame ();
	}

	return 0;
}
