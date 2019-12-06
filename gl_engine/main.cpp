#include <SDL.h>
#include <vector>

#include "Window.h"

#include "node/Node.h"
#include "node/MeshNode.h"
#include "shading/Shader.h"
#include "shading/Material.h"
#include "mesh/Mesh.h"
#include "mesh/Sphere.h"
#include "mesh/Arrow.h"
#include "mesh/Plane.h"
#include "mesh/obj.h"
#include "render/Renderer.h"
#include "Timer.h"


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
	GLuint width	= 800u;
	GLuint height	= 600u;
	Window window("GL Engine", st_config, width, height);

	CameraNode camNode1 = CameraNode("CamNode1");
	
	Material whiteMat = Material("Material", "LightShader.vert", "LightShader.frag");
	Material cShadMat = Material("cMat", "cShader.vert", "cShader.frag");

	Mesh arrow1 = Arrow::createArrow();
	MeshNode arrow1_node = MeshNode("Arrow1", &arrow1, &cShadMat);

	Mesh plane = Plane::createPlane(10.0f, 10.0f);
	MeshNode plane_node = MeshNode("Plane1", &plane, &whiteMat);

	MeshNode arrow2_node = MeshNode("Arrow2", &arrow1, &whiteMat);
	arrow2_node.setPosition({ 5.0, 0.0, 0.0 });
	arrow2_node.setParent(&arrow1_node);

	Renderer render = Renderer(&camNode1, glm::uvec2(width, height));
	render.addNode(&arrow1_node);
	render.addNode(&plane_node);

	Timer timer;

	while (render.pollEvents())
	{
		float new_y_rotation = arrow1_node.rotation().y + (float)timer.delta_time_s() * 10;
		arrow1_node.setRotation({ 0.0, new_y_rotation, 0.0 });

		glm::vec4 newColor = { 0.3, (std::cos(timer.total_time_s() * 5) + 1) / 2, 0.8, 1.0 };
		arrow1_node.material()->setUniform("uColor", newColor);

		render.update(&window, &timer);
	}

	return 0;
}
