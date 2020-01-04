#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <GL/glew.h>

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
	targetCam.set_position({ 0.0f, 8.0f, 8.0f });
	targetCam.set_clip_far(1000.0f);

	// Orthogonal Camera
	gl_engine::OrthoCamera orthoCam{};
	gl_engine::CameraNode orthoCam_node{ "Ortho Cam 1", &orthoCam };
	orthoCam.set_clip_near(0.001f);
	orthoCam.set_clip_far(100.0f);
	orthoCam.set_sides(-30.0f, 30.0f, -30.0f, 30.0f);

	// Free Camera
	gl_engine::FreeCamera freeCam{};
	gl_engine::CameraNode freeCam_node{ "Free Cam 1", &freeCam };
	freeCam.set_clip_near(0.001f);
	freeCam.set_clip_far(1000.0f);


	// Shader 1
	gl_engine::Material shaderBall_material{ "cMat", "cShader.vert", "cShader.frag" };
	shaderBall_material.setUniform("material.spec_power", 32.0f);
	shaderBall_material.setUniform("material.specular", glm::vec3(0.7, 0.6, 0.9));

	// Shader 2
	gl_engine::Material floor_material{ "floor material", "cShader.vert", "cShader.frag" };
	floor_material.setUniform("material.spec_power", 32.0f);
	floor_material.setUniform("material.specular", glm::vec3(0.7, 0.6, 0.9));

	// Texture 1
	gl_engine::Texture tex1("uvtemplateB.tga");
	shaderBall_material.addTexture("material.diffuse", &tex1);

	// Texture 2
	gl_engine::Texture tex2("greyGrid_01.tga");
	floor_material.addTexture("material.diffuse", &tex2);

	//shaderBall_material.addTexture("shadowMap", &tex2);


	// Texture 3
	//gl_engine::Texture arrayTest_tex("uvtemplate.tga");
	//arrayTest_tex.set_target(GL_TEXTURE_2D_ARRAY);
	//shaderBall_material.addTexture("shadowMap", &arrayTest_tex);

	// Mesh 1
	gl_engine::Mesh shaderBall = gl_engine::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");
	gl_engine::MeshNode shaderBall_node{ "shader ball", &shaderBall, &shaderBall_material };

	// Mesh 2
	gl_engine::Mesh plane = gl_engine::Plane::createPlane(100.0f, 100.0f, 1, 1);
	plane.scale_uvs(10.0f);
	gl_engine::MeshNode plane_node{ "Plane 1", &plane, &floor_material };

	// Mesh 3
	//gl_engine::Mesh cube = gl_engine::Cube::createCube(5.0f);
	//gl_engine::MeshNode cube_node{ "Cube 1", &cube, &cShadMat };
	//cube_node.setParent(&freeCam_node);
	//cube_node.set_position({ 0.0f, 3.0f, 6.0f });

	// Point Light 1
	gl_engine::PointLight pointLight{ 1.0f, { 0.0f, 0.0f, 0.0f } };
	pointLight.setBrightness(4.1f);
	pointLight.setColor(glm::vec3(1.0, 0.7, 0.2));
	pointLight.setRadius(0.1f);
	//pointLight.disable_mesh();
	gl_engine::LightNode pointLight_node{ "Point Light 1", &pointLight };
	pointLight_node.set_position({ -6.0f, 1.2f, 0.0f });

	// Point Light 2
	gl_engine::PointLight pointLight2{ 3.0f, { 0.2f, 0.1f, 1.0f } };
	gl_engine::LightNode pointLight_node2{ "Point Light 2", &pointLight2 };
	pointLight_node2.set_position({ 0.0f, 4.0f, -5.0f });

	// Directional Light 1
	gl_engine::DirectionalLight directionalLight1{ 0.5f, {1.0f, 1.0f, 1.0f} /*{ 0.2f, 1.0f, 0.1f }*/ };
	gl_engine::LightNode directionalLight_node1{ "Directional Light 1", &directionalLight1 };
	directionalLight_node1.setRotation({ 33.0f, 225.0f, 0.0f });
	directionalLight_node1.set_position({ 16.0f, 16.0f, 16.0f });

	directionalLight_node1.addChild(&freeCam_node);
	directionalLight_node1.addChild(&orthoCam_node);


	// Spot Light 1
	gl_engine::SpotLight spotLight1{ 10.0f, { 1.0f, 0.0f, 0.0f } };
	gl_engine::LightNode spotLight_node1{ "Spot Light 1", &spotLight1 };
	spotLight_node1.setRotation({ 60.0f, 0.0f, 0.0f });
	spotLight_node1.set_position({ -3.0f, 10.0f, -2.0f });
	spotLight1.set_innerAngle(30.0f);
	spotLight1.set_outerAngle(33.0f);

	// Spot Light 2
	gl_engine::SpotLight spotLight2{ 14.0f, {0.0f, 0.0f, 1.0f } };
	gl_engine::LightNode spotLight_node2{ "Spot Light 2", &spotLight2 };
	spotLight_node2.set_position({ 6.0f, 12.0f, 2.0f});
	spotLight_node2.setRotation({ 50.0f, -100.0f, 0.0f });
	spotLight2.set_innerAngle(25.0f);
	spotLight2.set_outerAngle(70.0f);

	// Shadow map
	gl_engine::ShadowMap shadowMap_spot1{ &spotLight_node1 };
	gl_engine::ShadowMap shadowMap_spot2{ &spotLight_node2 };
	gl_engine::ShadowMap shadowMap_directional1{ &directionalLight_node1 };
	gl_engine::ShadowMap shadowMap_point1{ &pointLight_node };
	gl_engine::ShadowMap shadowMap_point2{ &pointLight_node2 };

	// Null node 1
	gl_engine::Node lightRotate1{ "light rotate 01" };
	//lightRotate1.addChild(&pointLight_node);

	// Null node 2
	gl_engine::Node lightRotate2{ "light rotate 02" };
	lightRotate2.addChild(&lightRotate1);
	lightRotate2.set_position({ 2.0, 0.0, 0.0 });

	// Axis arrows
	gl_engine::Mesh axis_arrow{ gl_engine::Arrow::createArrow(20) };
	gl_engine::Material axis_mat_x{ "axis arrow x", "LightShader.vert", "LightShader.frag" };
	gl_engine::Material axis_mat_y{ "axis arrow x", "LightShader.vert", "LightShader.frag" };
	gl_engine::Material axis_mat_z{ "axis arrow x", "LightShader.vert", "LightShader.frag" };
	axis_mat_x.setUniform("light.color", glm::vec3{ 1.0f, 0.0f, 0.0f });
	axis_mat_y.setUniform("light.color", glm::vec3{ 0.0f, 1.0f, 0.0f });
	axis_mat_z.setUniform("light.color", glm::vec3{ 0.0f, 0.0f, 1.0f });
	gl_engine::MeshNode axis_arrow_x_node{ "axis arrow x node", &axis_arrow, &axis_mat_x };
	gl_engine::MeshNode axis_arrow_y_node{ "axis arrow y node", &axis_arrow, &axis_mat_y };
	gl_engine::MeshNode axis_arrow_z_node{ "axis arrow z node", &axis_arrow, &axis_mat_z };
	axis_arrow_x_node.setScale(0.05f);
	axis_arrow_y_node.setScale(0.05f);
	axis_arrow_z_node.setScale(0.05f);
	axis_arrow_x_node.set_position({ 1.0f, 0.0f, 0.0f });
	axis_arrow_y_node.set_position({ 0.0f, 1.0f, 0.05f });
	axis_arrow_z_node.set_position({ 0.0f, 0.0f, 1.0f });
	axis_arrow_x_node.setRotation({ 0.0f, 90.0f, 0.0f });
	axis_arrow_y_node.setRotation({ -90.0f, 0.0f, 0.0f });


	// Renderer
	gl_engine::Renderer render{ &targetCam_node, glm::uvec2(width, height) };
	render.addNode(&shaderBall_node);
	render.addNode(&plane_node);
	render.addNode(&pointLight_node);
	render.addNode(&pointLight_node2);
	render.addNode(&directionalLight_node1);
	render.addNode(&spotLight_node1);
	render.addNode(&spotLight_node2);
	//render.addNode(&axis_arrow_x_node);
	//render.addNode(&axis_arrow_y_node);
	//render.addNode(&axis_arrow_z_node);

	//tex1.unbind();

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

