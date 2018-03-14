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
//#define DEBUG


static constexpr auto POSITION_ATTR = 0u;
static constexpr auto COLOR_ATTR    = 1u;
static constexpr auto NORMAL_ATTR	= 2u;
static constexpr auto CAM_ATTR		= 3u;
static constexpr auto MODEL_ATTR    = 4u;
static constexpr auto WORLD_ATTR	= 8u;

struct ApplicationState {
	GLuint programID		= 0;

	//GLuint width			= 0;
	//GLuint height			= 0;

	GLuint numBuffers      = 1;

	//glm::mat4 view        = glm::mat4();
	//glm::mat4 projection  = glm::mat4();

	std::vector<glm::mat4>	modelMatrix;
	std::vector<GLfloat>	offsets;
	//Camera					cam;

	double time				= 0.0;
	double freqMultiplier	= 0.0;

	SDL_Window*		st_window = nullptr;
	SDL_GLContext	st_opengl = nullptr;

	ShapeGenerator	shapes;
	ShapeContainer  sh;

	Buffer geoBuffer		= { GL_ARRAY_BUFFER, 0 };
	Buffer matBuffer		= { GL_ARRAY_BUFFER, 0 };
	Buffer wldBuffer		= {	GL_ARRAY_BUFFER, 0 };
	Buffer indxBuffer		= { GL_ARRAY_BUFFER, 0 };
	Buffer atomicsBuffer	= { GL_ATOMIC_COUNTER_BUFFER, 0 };

	VAO    VAO_main, VAO_mat;
	
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

void initLight(ApplicationState& _State)
{
	// // Set up lights // //
}

void initGeo(ApplicationState& _State, GL_Scene& _Scene)
{
	// // Create and compile our GLSL program from the shaders // //
	_State.programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");

	// // Create Geo
	_State.sh.appendShape(_State.shapes.makePlane(2), "plane_01"); 
	_State.sh.appendShape(_State.shapes.makePlane(2), "plane_02");
	_State.sh.appendShape(_State.shapes.makePlane(2), "plane_03");

	// // Create transforms
	glm::vec3 planeScale = glm::vec3(3.0f);

	_State.sh.appendTransform(Utilities::trs(glm::mat3({ 0.0f, -1.0f, 0.0 }, { 0.0f, 0.0f, 0.0f }, planeScale)), "transform_01");
	_State.sh.appendTransform(Utilities::trs(glm::mat3({ 0.0f, -3.0f, 0.0 }, { 0.0f, 0.0f, 0.0f }, planeScale)), "transform_02");
	_State.sh.appendTransform(Utilities::trs(glm::mat3({ 0.0f, -5.0f, 0.0 }, { 0.0f, 0.0f, 0.0f }, planeScale)), "transform_03");	


	// // Transform Geo 
	_State.sh.connect("transform_01",		"plane_01");
	_State.sh.connect("transform_02",		"plane_02");
	_State.sh.connect("transform_03",		"plane_03");

	// // Send information to graphics card
	_State.geoBuffer.Append(_State.sh.vertices());
	//auto depthSortedIndices = _State.sh.depthSort(_State.cam.getPosition());
	//_State.indxBuffer.Append(_State.sh.depthSort(_State.cam.getPosition()));
	auto depthSortedIndices = _State.sh.depthSort(_Scene.m_cam.getPosition());
	_State.indxBuffer.Append( _State.sh.depthSort(_Scene.m_cam.getPosition()));


	//_State.matBuffer.Append(sizeof(glm::mat4), &(_State.projection * _State.cam.getWorldToViewMatrix()[0][0]));
	_State.matBuffer.Append(sizeof(glm::mat4), &(_Scene.m_projection * _Scene.m_cam.getWorldToViewMatrix()[0][0]));
	_State.wldBuffer.Append(sizeof(glm::mat4), &glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)));


	// // Set up standard information for the VAO
	static const auto shape_info  = gl_introspect_tuple<std::tuple<glm::vec3, glm::vec3, glm::vec3, GLint>>::get();
	static const auto matrix_info = gl_introspect_tuple<std::tuple<glm::mat4>>::get();

	// // Upload the VAO information
	_State.VAO_main.GenerateVAO(_State.geoBuffer, 0, shape_info.data(),  shape_info.data()  + shape_info.size(),  POSITION_ATTR);
	_State.VAO_main.GenerateVAO(_State.matBuffer, 1, matrix_info.data(), matrix_info.data() + matrix_info.size(), MODEL_ATTR);
	_State.VAO_main.GenerateVAO(_State.wldBuffer, 1, matrix_info.data(), matrix_info.data() + matrix_info.size(), WORLD_ATTR);

	_State.sh.uploadTransforms(_State.programID);
	_State.sh.uploadConnections(_State.programID);

#ifdef DEBUG
	 // Debug matrix array
	std::vector<glm::mat4> testTransformsUniform;
	testTransformsUniform = _State.sh.readUniform<glm::mat4>(_State.programID, "transforms", 4);
	std::vector<glm::ivec3> testConnectionsUniform;
	testConnectionsUniform = _State.sh.readUniform<glm::ivec3>(_State.programID, "connections", 7);


#endif // DEBUG

auto x = 3;

	return;
}

void finish_frame (ApplicationState& _State)
{
	SDL_GL_SwapWindow (_State.st_window);
}

void prepareLight(ApplicationState& _State) 
{
	// // Ambient Lighting // //
	//glm::vec4 ambientLight = { 0.0f, 0.34f, 0.6f, 1.0f };
	//glUniform4fv(_State.ambientLightID, 1, &ambientLight.r);
	// // Diffuse Lighting // // 
	//glm::vec3 lightPosition = { 0.0f, 2.0f, 0.0f };
	//glUniform3fv(_State.lightPositionID, 1, &lightPosition.x);
}

void prepareCam(ApplicationState& _State)
{
	// // Cam position // //
	//glm::vec3 camPositionVec = _State.cam.getPosition();
	//glUniform3fv(_State.camPositionID, 1, &camPositionVec.x);
}

void prepareGeo(ApplicationState& _State, GL_Scene& _Scene)
{
	// // Update matrices // //
	//_State.matBuffer.Upload( _State.projection * _State.cam.getWorldToViewMatrix());
	//_State.indxBuffer.Upload(_State.sh.depthSort(_State.cam.getPosition()));
	_State.matBuffer.Upload( _Scene.m_projection * _Scene.m_cam.getWorldToViewMatrix());
	_State.indxBuffer.Upload(_State.sh.depthSort(_Scene.m_cam.getPosition()));
}

void readAtomic(ApplicationState& _State)
{
	GLuint initZeros[] = { 8, 8, 8 };
	_State.atomicsBuffer.ReadBuffer((void*)initZeros);
	auto x = 0;
}

void render_frame (ApplicationState& _State, GL_Scene& _Scene)
{
	// // Tutorial from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ // //
	// // Clear both the colour buffer and the depth buffer at the same time // //
	 glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	 //Get time
	 _State.time    = _State.freqMultiplier * SDL_GetPerformanceCounter();

	 //Colour multiplier based on time and a cosine wave
	 GLfloat cmAmplitude = 0.5f;
	 GLfloat cmFreq      = 0.1f;
	 GLfloat cmOffset    = 0.5f;

	 prepareCam(_State);
	 prepareLight(_State);
	 prepareGeo(_State, _Scene);
	 readAtomic(_State);
	 
	 _State.VAO_main.Bind();
	 _State.indxBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
	 glDrawElements(GL_TRIANGLES, (GLsizei)_State.indxBuffer.size(), GL_UNSIGNED_SHORT, 0);
}


void exit(ApplicationState &_State){
	glUseProgram(0);
	glDeleteProgram(_State.programID);
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
				//_State.wldBuffer.ReadBuffer(&wldBuffer[0][0]);
				//_State.cam.focus(wldBuffer);
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

	//_State.cam.moveRel(axisDelta * cMoveSpeed);
	//_State.cam.rotateRel(rotateDelta * cRotateSpeed);
	_Scene.m_cam.moveRel(axisDelta * cMoveSpeed);
	_Scene.m_cam.rotateRel(rotateDelta * cRotateSpeed);


}


int main(int, char**)
{
	GL_Scene			_Scene;
	ApplicationState	_State;

	initWindow(	_State, _Scene);
	initLight(	_State);
	initGeo(	_State, _Scene);
	while (poll_events (_State, _Scene))
	{
		update_camera(_State, _Scene);
		render_frame (_State, _Scene);
		finish_frame (_State);
	}

	exit(_State);
	return 0;
}
