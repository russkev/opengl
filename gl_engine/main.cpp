#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "Window.h"

#include "node/Node.h"
#include "node/MeshNode.h"
#include "node/LightNode.h"
#include "node/CameraNode.h"
#include "shading/Shader.h"
#include "shading/Material.h"
#include "mesh/Mesh.h"
#include "mesh/Sphere.h"
#include "mesh/Arrow.h"
#include "mesh/Plane.h"
#include "mesh/obj.h"
#include "light/PointLight.h"
#include "render/Renderer.h"
#include "shading/Texture.h"
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
	
	Material cShadMat = Material("cMat", "cShader.vert", "cShader.frag");
	cShadMat.setUniform("material.spec_power", 32.0f);
	//cShadMat.setUniform("material.diffuse", glm::vec3(0.8, 0.9, 0.7));
	cShadMat.setUniform("material.specular", glm::vec3(0.7, 0.6, 0.9));

	Texture tex1("uvtemplate.tga");
	cShadMat.setTexture("material.diffuse", tex1);

	Texture tex2("uvtemplate.tga");

	Mesh shaderBall = OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");
	MeshNode shaderBall_node = MeshNode("shader ball", &shaderBall, &cShadMat);
	//shaderBall_node.setScale({ 1.0, 10.0, 1.0 });

	Mesh plane = Plane::createPlane(20.0f, 20.0f, 20, 20);
	MeshNode plane_node = MeshNode("Plane1", &plane, &cShadMat);

	// Light 1
	PointLight pointLight = PointLight(1.0f, { 1.0f, 0.0f, 0.0f });
	pointLight.setBrightness(2.1f);
	pointLight.setColor(glm::vec3(1.0, 0.7, 0.2));
	LightNode pointLight_node = LightNode("Point Light 1", &pointLight);
	pointLight_node.setPosition({ -6.0f, 1.2f, 0.0f });

	// Light 2
	PointLight pointLight2 = PointLight(3.0f, { 0.2f, 0.1f, 1.0f });
	LightNode pointLight_node2 = LightNode("Point Light 2", &pointLight2);
	pointLight_node2.setPosition({ 0.0f, 4.0f, -5.0f });

	Node lightRotate1 = Node("light rotate 01");
	//lightRotate1.addChild(&pointLight_node);

	Node lightRotate2 = Node("light rotate 02");
	lightRotate2.addChild(&lightRotate1);
	lightRotate2.setPosition({ 2.0, 0.0, 0.0 });

	Renderer render = Renderer(&camNode1, glm::uvec2(width, height));
	render.addNode(&shaderBall_node);
	render.addNode(&plane_node);
	render.addLightNode(&pointLight_node);
	render.addLightNode(&pointLight_node2);

	Timer timer;

	while (render.pollEvents())
	{
		float new_y_rotation = shaderBall_node.rotation().y + (float)timer.delta_time_s() * 30;
		shaderBall_node.setRotation({ 0.0, new_y_rotation, 0.0 });

		//float l1_rotate = lightRotate1.rotation().z + (float)timer.delta_time_s() * 100;
		//lightRotate1.setRotation({ 0.0 , 0.0, l1_rotate });

		//float l2_rotate = lightRotate2.rotation().y - (float)timer.delta_time_s() * 60;
		//lightRotate2.setRotation({ 0.0, l2_rotate, 0.0 });

		render.update(&window, &timer);
	}

	return 0;
}
