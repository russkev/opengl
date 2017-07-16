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

	GLuint TheBufferID			= 0;
	GLuint CubeVertexArrayID	= 0;
	GLuint PlaneVertexArrayID	= 0;
	GLuint ArrowVertexArrayID	= 0;
	GLuint PlaneNormalsVertexArrayID = 0;
	GLuint ArrowNormalsVertexArrayID = 0;
	GLuint MatrixBufferID		= 0;
	GLuint WorldMatBuffID		= 0;
	GLuint NormalsID			= 0;
	GLuint ColorBufferID		= 0;
	GLuint CamPositionBufferID	= 0;
	std::vector<GLuint> VertexArrays;

	GLuint numBuffers      = 1;
	GLuint numInstances    = 0;
	GLuint cubeNumIndices  = 0;
	GLuint arrowNumIndices = 0;
	GLuint planeNumIndices = 0;
	GLuint normalsPlaneNumIndices = 0;
	GLuint normalsArrowNumIndices = 0;

	GLsizeiptr sizeOfArrow      = 0;
	GLsizeiptr sizeOfCube       = 0;
	GLsizeiptr sizeOfPlane		= 0;
	GLsizeiptr sizeOfArrowVerts = 0;
	GLsizeiptr sizeOfCubeVerts  = 0;
	GLsizeiptr sizeOfPlaneVerts = 0;
	GLsizeiptr sizeOfPlaneNormals    = 0;
	GLsizeiptr sizeOfPlaneNormalsVerts = 0;
	GLsizeiptr sizeOfArrowNormals = 0;
	GLsizeiptr sizeOfArrowNormalsVerts = 0;

	glm::mat4 view        = glm::mat4();
	glm::mat4 projection  = glm::mat4();
	std::vector<glm::mat4> modelMatrix;
	std::vector<GLfloat> offsets;
	Camera cam;

	double time = 0.0;
	double freqMultiplier = 0.0;

	SDL_Window*		st_window = nullptr;
	SDL_GLContext	st_opengl = nullptr;

	ShapeData sData_plane;
	ShapeData sData_arrow;

	Buffer vertexBuffer			= { GL_ARRAY_BUFFER, 0 };
	Buffer viewMatrixBuffer		= { GL_ARRAY_BUFFER, 0 };
	Buffer worldMatrixBuffer	= { GL_ARRAY_BUFFER, 0 };
	
	
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
	static ShapeData data_triangle			= ShapeGenerator::makeTriangle();
	static ShapeData data_plane				= ShapeGenerator::makePlane(3);
	static ShapeData data_arrow				= ShapeGenerator::makeArrow();
	static ShapeData data_plane_normals		= ShapeGenerator::makeNormals(data_plane);
	static ShapeData data_arrow_normals		= ShapeGenerator::makeNormals(data_arrow);

	_State.sData_plane = data_plane;
	_State.sData_arrow = data_arrow;


	// // Set up global variables for size of various elements // //
	_State.arrowNumIndices		  = data_arrow.numIndices();
	_State.planeNumIndices		  = data_plane.numIndices();
	_State.normalsPlaneNumIndices = data_plane_normals.numIndices();
	_State.normalsArrowNumIndices = data_arrow_normals.numIndices();

	_State.sizeOfPlane				= data_plane.sizeVertices() + data_plane.sizeIndices();
	_State.sizeOfPlaneVerts			= data_plane.sizeVertices();
	_State.sizeOfPlaneNormals		= data_plane_normals.sizeVertices() + data_plane_normals.sizeIndices();
	_State.sizeOfPlaneNormalsVerts	= data_plane_normals.sizeVertices();
	_State.sizeOfArrow				= data_arrow.sizeVertices() + data_arrow.sizeIndices();
	_State.sizeOfArrowVerts			= data_arrow.sizeVertices();
	_State.sizeOfArrowNormals		= data_arrow_normals.sizeVertices() + data_arrow_normals.sizeIndices();
	_State.sizeOfArrowNormalsVerts	= data_arrow_normals.sizeVertices();


	// // TEST // //
	//loadBMP_custom BMP1("uvtemplate.bmp");
	// // END TEST // //

	// // Push plane and arrow vertices to :graphics card memory (location: TheBufferID):
	glGenBuffers(1, &_State.TheBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.TheBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		_State.sizeOfPlane +
		_State.sizeOfPlaneNormals +
		_State.sizeOfArrow +
		_State.sizeOfArrowNormals,
		nullptr, GL_STATIC_DRAW);

	std::vector<ShapeData> shapes = { ShapeGenerator::makePlane(1)/*, ShapeGenerator::makePlane(1) */};
	_State.vertexBuffer.createGeoBuffer(shapes);

	glBindBuffer(GL_ARRAY_BUFFER, _State.TheBufferID);
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, data_plane.sizeVertices(), &data_plane.vertices.front());
	currentOffset += data_plane.sizeVertices();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, data_plane.sizeIndices(), &data_plane.indices.front());
	currentOffset += data_plane.sizeIndices();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, data_plane_normals.sizeVertices(), &data_plane_normals.vertices.front());
	currentOffset += data_plane_normals.sizeVertices();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, data_plane_normals.sizeIndices(), &data_plane_normals.indices.front());
	currentOffset += data_plane_normals.sizeIndices();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, data_arrow.sizeVertices(), &data_arrow.vertices.front());
	currentOffset += data_arrow.sizeVertices();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, data_arrow.sizeIndices(), &data_arrow.indices.front());
	currentOffset += data_arrow.sizeIndices();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, data_arrow_normals.sizeVertices(), &data_arrow_normals.vertices.front());
	currentOffset += data_arrow_normals.sizeVertices();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, data_arrow_normals.sizeIndices(), &data_arrow_normals.indices.front());

	// // The Vertex array object stores information about what the buffer actually contains // //

	glGenVertexArrays(1, &_State.PlaneVertexArrayID);
	_State.VertexArrays.push_back(_State.PlaneVertexArrayID);
	glGenVertexArrays(1, &_State.PlaneNormalsVertexArrayID);
	_State.VertexArrays.push_back(_State.PlaneNormalsVertexArrayID);
	glGenVertexArrays(1, &_State.ArrowVertexArrayID);
	_State.VertexArrays.push_back(_State.ArrowVertexArrayID);
	glGenVertexArrays(1, &_State.ArrowNormalsVertexArrayID);
	_State.VertexArrays.push_back(_State.ArrowNormalsVertexArrayID);

	{
		GLsizeiptr offset = 0;
		for (auto i : _State.VertexArrays) {
			glBindVertexArray(i);
			glEnableVertexAttribArray(POSITION_ATTR);
			glVertexAttribPointer(POSITION_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offset);
			glEnableVertexAttribArray(COLOR_ATTR);
			glVertexAttribPointer(COLOR_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offset + sizeof(glm::tvec3<GLfloat>)));
			glEnableVertexAttribArray(NORMAL_ATTR);
			glVertexAttribPointer(NORMAL_ATTR, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offset + (sizeof(glm::tvec3<GLfloat>) * 2)));
			if (i == _State.PlaneVertexArrayID) { offset += _State.sizeOfPlane; }
			if (i == _State.PlaneNormalsVertexArrayID) { offset += _State.sizeOfPlaneNormals; }
			if (i == _State.ArrowVertexArrayID) { offset += _State.sizeOfArrow; }
			if (i == _State.ArrowNormalsVertexArrayID) { offset += _State.sizeOfArrowNormals; }
		}
	}


	// // Push offset information to the graphics card memory
	// // Make a line of evenly spaced cubes from line start to line finish
	GLfloat lineStart = -15.0f;
	GLfloat lineEnd = +15.0f;
	_State.numInstances = 10;
	GLfloat distance = lineEnd - lineStart;
	GLfloat spacing = (lineEnd - lineStart) / _State.numInstances;
	while (lineStart <= lineEnd) {
		_State.offsets.push_back(lineStart);
		lineStart += spacing;
	}

	// Initial matrix
	// Plane
	_State.modelMatrix.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	// Normals
	_State.modelMatrix.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	for (GLuint i = 0; i < _State.numInstances; ++i) {
		if (i == 0) {
			_State.modelMatrix.push_back(glm::mat4());
		}
		else {
			_State.modelMatrix.push_back(glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f),
				glm::vec3(0.0f + _State.offsets.at(i), 2.0f, 0.0f)),					//Translate
				(rand() / (float)RAND_MAX)*360, glm::vec3(0.0f, 1.0f, 1.0f)),			//Random Rotate
				//0.0f, glm::vec3(0.0f, 1.0f, 1.0f)),									//Non Random Rotate
				glm::vec3(0.1f, 0.1f, 0.1f)));											//Scale
		}
	}


	glGenBuffers(1, &_State.MatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.MatrixBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*_State.modelMatrix.size(), _State.modelMatrix.data(), GL_DYNAMIC_DRAW);
	for (auto& j : _State.VertexArrays) {
		glBindVertexArray(j);
		for (int i = 0; i < 4; ++i) {
			glEnableVertexAttribArray(	MODEL_ATTR + i);
			glVertexAttribPointer(		MODEL_ATTR + i, 4u, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * (i * 4)));
			glVertexAttribDivisor(		MODEL_ATTR + i, 1);
		}
	}


	glGenBuffers(1, &_State.WorldMatBuffID);
	glBindBuffer(GL_ARRAY_BUFFER, _State.WorldMatBuffID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _State.modelMatrix.size(), _State.modelMatrix.data(), GL_DYNAMIC_DRAW);
	for (auto& j : _State.VertexArrays) {
		glBindVertexArray(j);
		for (int i = 0; i < 4; ++i) {
			glEnableVertexAttribArray(	WORLD_ATTR + i);
			glVertexAttribPointer(		WORLD_ATTR + i, 4u, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * (i * 4)));
			glVertexAttribDivisor(		WORLD_ATTR + i, 1);	
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

	 _State.vertexBuffer.drawGeo(_State.cam, _State.projection);
}


void exit(ApplicationState &_State){
	glDeleteBuffers(_State.numBuffers, &_State.TheBufferID);
	glDeleteVertexArrays(_State.numBuffers, &_State.ArrowVertexArrayID);
	glDeleteVertexArrays(_State.numBuffers, &_State.PlaneVertexArrayID);
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
