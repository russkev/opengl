#include <SDL.h>
#include <vector>

#include "Window.h"
#include "GL_Scene.h"

#include "node/Node.h"
#include "node/MeshNode.h"
#include "shading/Shader.h"
#include "shading/Material.h"
#include "mesh/Mesh.h"
#include "mesh/Sphere.h"


#define GLM_ENABLE_EXPERIMENTAL
//#define DEBUG

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
	Window			_Window;
	GL_Scene		_Scene;

	GLuint width	= 800u;
	GLuint height	= 600u;

	_Window.init(st_config, width, height);

	/*

	Testing area

	*/

	//Material mat1("mat1", "SimpleVertexShader.vert", "SimpleFragmentShader.frag");
	//Mesh mesh1 = Sphere::createSphere(3.0f);
	//MeshNode node1("sphere1", &mesh1, &mat1);


	/*

		End testing area

	*/
	


	_Scene.init(width, height);

	while (_Scene.pollEvents())
	{
		_Scene.m_cam.update();
		_Scene.renderFrame();
		_Window.finish_frame();
	}
	return 0;
}
