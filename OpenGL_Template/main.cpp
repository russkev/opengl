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
#include "GLShapes.h"
#include "Buffer.h"


#define GLM_ENABLE_EXPERIMENTAL
#define DEBUG
#include <glm/gtc/matrix_transform.hpp>

static constexpr auto POSITION_ATTR = 0u;
static constexpr auto COLOR_ATTR    = 1u;
static constexpr auto NORMAL_ATTR	= 2u;
static constexpr auto CAM_ATTR		= 3u;
static constexpr auto MODEL_ATTR    = 4u;
static constexpr auto WORLD_ATTR	= 8u;

struct ApplicationState {
	GLuint programID       = 0;
	GLuint matrixID        = 0;
	GLuint ambientLightID  = 0;
	GLuint lightPositionID = 0;
	GLuint worldMatrixID   = 0;
	GLuint camPositionID   = 0;

	GLuint numBuffers      = 1;

	glm::mat4 view        = glm::mat4();
	glm::mat4 projection  = glm::mat4();
	std::vector<glm::mat4> modelMatrix;
	std::vector<GLfloat> offsets;
	Camera cam;

	double time = 0.0;
	double freqMultiplier = 0.0;

	SDL_Window*		st_window = nullptr;
	SDL_GLContext	st_opengl = nullptr;

	GLShapes vertexBuffer	= { GL_ARRAY_BUFFER };
	GLShapes normalsBuffer	= { GL_ARRAY_BUFFER };
	Buffer geoBuffer		= { GL_ARRAY_BUFFER, 0 };
	Buffer matBuffer		= { GL_ARRAY_BUFFER, 0 };
	Buffer wldBuffer		= {	GL_ARRAY_BUFFER, 0 };
	
	
	ApplicationState() {
		if (st_window) SDL_DestroyWindow(st_window);
		if (st_opengl) SDL_GL_DeleteContext(st_opengl);
	}

	// // TEST // //
	//ShapeData test_plane = ShapeGenerator::makePlane(2);
	//std::size_t test_plane_size = test_plane.sizeShape();
	//Buffer ba = Buffer(GL_ARRAY_BUFFER, test_plane_size);
	// // END TEST // //

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
	// // Enable backface culling // //
	glEnable(GL_CULL_FACE);
	// // Set windind direction // // 
	glFrontFace(GL_CCW);
	// // Accept fragment shader if it closer to the camera than the previous one
	glDepthFunc(GL_LESS);

	// // Create and compile our GLSL program from the shaders // //
	_State.programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");

	// // Fetch uniforms from vertex shader // //
	_State.matrixID = glGetUniformLocation(_State.programID, "MVP");
	_State.ambientLightID = glGetUniformLocation(_State.programID, "ambientLight");
	_State.lightPositionID = glGetUniformLocation(_State.programID, "lightPosition");
	_State.worldMatrixID = glGetUniformLocation(_State.programID, "ModelToWorldMatrix");
	_State.camPositionID = glGetUniformLocation(_State.programID, "camPosition");

	// // Set up camera // //
	_State.projection = glm::perspective(glm::radians(50.0f), float(width) / float(height), 0.1f, 100.0f);
	_State.view       = _State.cam.getWorldToViewMatrix();

	// // Create 3D models
	//static ShapeData data_arrow_normals		= ShapeGenerator::makeNormals(data_arrow);

	// // TEST // //

	ShapeData test_plane = ShapeGenerator::makePlane(2);
	std::size_t test_plane_size = test_plane.sizeShape();
	std::uint32_t planeVerticesOffset = _State.geoBuffer.Append(test_plane.sizeVertices(), test_plane.vertices.data());
	std::uint32_t planeIndicesOffset  = _State.geoBuffer.Append(test_plane.sizeIndices(), test_plane.indices.data());
	_State.matBuffer.Append(sizeof(glm::mat4), &glm::mat4(1.0)[0][0]);
	_State.wldBuffer.Append(sizeof(glm::mat4), &glm::mat4(1.0)[0][0]);

	// // END TEST // //


	glm::mat4 testMatrix1 = glm::mat4(1.0);
	glm::mat4 testMatrix2 = glm::translate(testMatrix1, glm::vec3(0.0, 3.0, 0.0));
	testMatrix1[3][1] = 8.0f;

	std::vector<glm::mat4> arrowTransforms = {
		 glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f,  0.0f)), 3.14f,     glm::vec3(0.25f, 1.0f, 0.0f))
		,glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(+0.0f, 0.0f, -3.0f)), 3.14f / 2, glm::vec3(0.25f, 1.0f, 0.25f))
		,glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(+3.0f, 0.0f, -0.0f)), 3.14f / 8, glm::vec3(0.00f, 0.0f, 1.0f))
		,glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(+0.0f, 0.0f, +3.0f)), 3.14f / 2, glm::vec3(-0.25f, -1.0f, 0.25f))
	};

	std::vector<glm::mat4> tubeTransforms = {
		glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f,  0.0f)), 3.14f * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f))
	};

	
	//_State.vertexBuffer.addShape(ShapeGenerator::makeArrow(), arrowTransforms);
	//_State.vertexBuffer.addShape(ShapeGenerator::makePlane(10));
	//_State.vertexBuffer.addShape(ShapeGenerator::makeCube());
	_State.vertexBuffer.addShape(ShapeGenerator::makeTube(10, 1.0f, 10.0f), tubeTransforms);
	_State.vertexBuffer.createGeoBuffer();
	//_State.normalsBuffer.addShape(ShapeGenerator::makeNormals(ShapeGenerator::makeTube(10, 1.0f, 10.0f)), tubeTransforms);
	//_State.normalsBuffer.createGeoBuffer();

	std::uint32_t planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	std::size_t offset = 0;
	glEnableVertexAttribArray(POSITION_ATTR);
	glVertexAttribPointer(POSITION_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(planeVerticesOffset + offset));
	offset += sizeof(glm::tvec3<GLfloat>);
	glEnableVertexAttribArray(COLOR_ATTR);
	glVertexAttribPointer(COLOR_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(planeVerticesOffset + offset));
	offset += sizeof(glm::tvec3<GLfloat>);
	glEnableVertexAttribArray(NORMAL_ATTR);
	glVertexAttribPointer(NORMAL_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(planeVerticesOffset + offset));

	return;
}

void finish_frame (ApplicationState& _State)
{
	SDL_GL_SwapWindow (_State.st_window);
}

void render_frame (ApplicationState& _State)
{
	// // Tutorial from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ // //
	// // Clear both the colour buffer and the depth buffer at the same time // //
	 glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	 //Get time
	 _State.time    = _State.freqMultiplier * SDL_GetPerformanceCounter();
	 const auto tpi = 0;// glm::pi<float>()*float(_State.time);

	 //Colour multiplier based on time and a cosine wave
	 GLfloat cmAmplitude = 0.5f;
	 GLfloat cmFreq      = 0.1f;
	 GLfloat cmOffset    = 0.5f;
	 //GLfloat colMult     = cmAmplitude * cos(2.0f * glm::pi<float>() * cmFreq * GLfloat(_State.time)) + cmOffset;

	 // // LIGHTING // //
	 glUseProgram(_State.programID);
	 // // Ambient Lighting // //
	 glm::vec4 ambientLight = { 0.0f, 0.17f, 0.3f, 1.0f };
	 glUniform4fv(_State.ambientLightID, 1, &ambientLight.r);
	 // // Diffuse Lighting // // 
	 glm::vec3 lightPosition = { 0.0f, 2.0f, 0.0f };
	 glUniform3fv(_State.lightPositionID, 1, &lightPosition.x);

	 // // Cam position // //
	 glm::vec3 camPositionVec = _State.cam.getPosition();
	 glUniform3fv(_State.camPositionID, 1, &camPositionVec.x);

	 //// Matrix transformations
	 //std::vector<glm::mat4> MVP;
	 std::vector<glm::mat4> MV;

	 _State.vertexBuffer.drawGeo(_State.cam, _State.projection, GL_TRIANGLES);
	 //_State.normalsBuffer.drawGeo(_State.cam, _State.projection, GL_LINES);
}


void exit(ApplicationState &_State){
	glUseProgram(0);
	glDeleteProgram(_State.programID);
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
