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
#include "camera/Camera.h"
#include "camera/TargetCamera.h"
#include "camera/OrthoCamera.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "light/SpotLight.h"
#include "render/Renderer.h"
#include "shading/Texture.h"
#include "Timer.h"


#define GLM_ENABLE_EXPERIMENTAL
//#define DEBUG

std::vector<gl_engine::opengl_attr_pair> st_config =
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
	gl_engine::Window window{ "GL Engine", st_config, width, height };

	// Camera
	gl_engine::TargetCamera targetCam{};
	targetCam.setClipFar(1000.0f);

	gl_engine::OrthoCamera orthoCam{};
	gl_engine::CameraNode camNode1{ "Target Camera 1", &targetCam };
	gl_engine::CameraNode orthoCamNode1{ "Ortho Cam 1", &orthoCam };
	//camNode1.setPosition(glm::vec3{ 0.0f, -4.0f, -6.0f });
	//camNode1.setRotation(glm::vec3{ 50.0f, 0.0f, 0.0f });
	orthoCam.setClipNear(0.001f);
	orthoCam.setClipFar(100.0f);
	orthoCam.setSides(-30.0f, 30.0f, -30.0f, 30.0f);
	orthoCamNode1.setPosition({ 0.0f, 0.0f, 0.0f });
	orthoCamNode1.setRotation({ -30.0f, 100.0f, 0.0f });
	//CameraNode camNode1 = CameraNode( "Camera Node 1", &targetCam );
	
	// Shader 1
	gl_engine::Material cShadMat{ "cMat", "cShader.vert", "cShader.frag" };
	cShadMat.setUniform("material.spec_power", 32.0f);
	cShadMat.setUniform("material.specular", glm::vec3(0.7, 0.6, 0.9));

	// Texture 1
	gl_engine::Texture tex1("uvtemplate.tga");
	cShadMat.setTexture("material.diffuse", tex1);

	// Texture 2
	gl_engine::Texture tex2("uvtemplate.tga");

	// Mesh 1
	gl_engine::Mesh shaderBall = gl_engine::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");
	gl_engine::MeshNode shaderBall_node{ "shader ball", &shaderBall, &cShadMat };

	// Mesh 2
	gl_engine::Mesh plane = gl_engine::Plane::createPlane(100.0f, 100.0f, 1, 1);
	gl_engine::MeshNode plane_node{ "Plane1", &plane, &cShadMat };

	// Light 1
	gl_engine::PointLight pointLight{ 1.0f, { 0.0f, 0.0f, 0.0f } };
	pointLight.setBrightness(2.1f);
	pointLight.setColor(glm::vec3(1.0, 0.7, 0.2));
	pointLight.disable_mesh();
	gl_engine::LightNode pointLight_node{ "Point Light 1", &pointLight };
	pointLight_node.setPosition({ -6.0f, 1.2f, 0.0f });

	// Light 2
	gl_engine::PointLight pointLight2{ 3.0f, { 0.2f, 0.1f, 1.0f } };
	gl_engine::LightNode pointLight_node2{ "Point Light 2", &pointLight2 };
	pointLight_node2.setPosition({ 0.0f, 4.0f, -5.0f });

	// Light 3
	gl_engine::DirectionalLight directionalLight1{ 0.2f, { 0.2f, 1.0f, 0.1f } };
	gl_engine::LightNode directionalLight_node1{ "Directional Light 1", &directionalLight1 };
	//directionalLight_node1.setPosition({ 8.0f, 9.0f, 0.0f });
	//directionalLight_node1.setRotation({ 30.0f, -100.0f, 0.0f });
	//directionalLight_node1.addChild(&orthoCamNode1);
	targetCam.setPosition({ -1.0,0,0 });
	targetCam.setFocusTarget({ 0.0f, 6.0f, 0.0f });
	//targetCam.setViewDirection({ -1.0, 0.0, 0.0 });
	directionalLight_node1.addChild(&camNode1);

	// Light 4
	gl_engine::SpotLight spotLight1{ 6.0f, { 1.0f, 0.0f, 0.0f } };
	gl_engine::LightNode spotLight_node1{ "Spot Light 1", &spotLight1 };
	spotLight_node1.setRotation({ 60.0f, 0.0f, 0.0f });
	spotLight_node1.setPosition({ -3.0f, 10.0f, -2.0f });
	spotLight1.set_innerAngle(30.0f);
	spotLight1.set_outerAngle(33.0f);

	// Null node 1
	gl_engine::Node lightRotate1{ "light rotate 01" };
	//lightRotate1.addChild(&pointLight_node);

	// Null node 2
	gl_engine::Node lightRotate2{ "light rotate 02" };
	lightRotate2.addChild(&lightRotate1);
	lightRotate2.setPosition({ 2.0, 0.0, 0.0 });

	// Renderer
	gl_engine::Renderer render{ &camNode1, glm::uvec2(width, height) };
	render.addNode(&shaderBall_node);
	render.addNode(&plane_node);
	render.addLightNode(&pointLight_node);
	render.addLightNode(&pointLight_node2);
	render.addLightNode(&directionalLight_node1);
	render.addLightNode(&spotLight_node1);


	gl_engine::Timer timer;

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

