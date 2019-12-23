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
#include "mesh/Cube.h"
#include "mesh/obj.h"
#include "camera/Camera.h"
#include "camera/TargetCamera.h"
#include "camera/OrthoCamera.h"
#include "camera/FreeCamera.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "light/SpotLight.h"
#include "render/Renderer.h"
#include "shading/ShadowMap.h"
#include "shading/Texture.h"
#include "utils/Timer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

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

	// Target Camera
	gl_engine::TargetCamera targetCam{};
	gl_engine::CameraNode targetCam_node{ "Target Camera 1", &targetCam };
	targetCam.setPosition({ 0.0f, 8.0f, 8.0f });
	targetCam.setClipFar(1000.0f);

	// Orthogonal Camera
	gl_engine::OrthoCamera orthoCam{};
	gl_engine::CameraNode orthoCam_node{ "Ortho Cam 1", &orthoCam };
	orthoCam.setClipNear(0.001f);
	orthoCam.setClipFar(100.0f);
	orthoCam.setSides(-30.0f, 30.0f, -30.0f, 30.0f);

	// Free Camera
	gl_engine::FreeCamera freeCam{};
	gl_engine::CameraNode freeCam_node{ "Free Cam 1", &freeCam };
	freeCam.setClipNear(0.001f);
	freeCam.setClipFar(1000.0f);


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
	gl_engine::MeshNode plane_node{ "Plane 1", &plane, &cShadMat };

	// Mesh 3
	//gl_engine::Mesh cube = gl_engine::Cube::createCube(5.0f);
	//gl_engine::MeshNode cube_node{ "Cube 1", &cube, &cShadMat };
	//cube_node.setParent(&freeCam_node);
	//cube_node.setPosition({ 0.0f, 3.0f, 6.0f });

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
	gl_engine::DirectionalLight directionalLight1{ 0.5f, { 0.2f, 1.0f, 0.1f } };
	gl_engine::LightNode directionalLight_node1{ "Directional Light 1", &directionalLight1 };
	directionalLight_node1.setRotation({ 33.0f, 225.0f, 0.0f });
	directionalLight_node1.setPosition({ 16.0f, 16.0f, 16.0f });

	directionalLight_node1.addChild(&freeCam_node);
	directionalLight_node1.addChild(&orthoCam_node);

	// Shadow map
	gl_engine::ShadowMap shadowMap{};
	directionalLight_node1.set_shadowMap(&shadowMap);


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
	gl_engine::Renderer render{ &targetCam_node, glm::uvec2(width, height) };
	render.addNode(&shaderBall_node);
	render.addNode(&plane_node);
	render.addNode(&pointLight_node);
	render.addNode(&pointLight_node2);
	render.addNode(&directionalLight_node1);
	render.addNode(&spotLight_node1);


	gl_engine::Timer timer;

	while (render.pollEvents())
	{
		float new_y_rotation = (float)shaderBall_node.rotation().y + (float)timer.delta_time_s() * 10.0f;
		shaderBall_node.setRotation({ 0.0, new_y_rotation, 0.0 });

		//float l1_rotate = lightRotate1.rotation().z + (float)timer.delta_time_s() * 100;
		//lightRotate1.setRotation({ 0.0 , 0.0, l1_rotate });

		//float l2_rotate = lightRotate2.rotation().y - (float)timer.delta_time_s() * 60;
		//lightRotate2.setRotation({ 0.0, l2_rotate, 0.0 });

		render.update(&window, &timer);
	}

	return 0;
}

