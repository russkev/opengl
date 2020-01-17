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

#include <glm/glm.hpp>
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

void spinning_shader_ball_scene(gl_engine::Window window);
void three_shader_ball_scene(gl_engine::Window window);

int main(int, char**)
{
	GLuint width	= 800u;
	GLuint height	= 600u;

	gl_engine::Window window{ "GL Engine", st_config, width, height };

	spinning_shader_ball_scene(window);
	//three_shader_ball_scene(window);

	return 0;
}

void spinning_shader_ball_scene(gl_engine::Window window)
{
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

	// Colors
	gl_engine::Texture white_tex{		glm::vec3{1.00f, 1.00f, 1.00f} };
	gl_engine::Texture light_grey_tex{	glm::vec3{0.85f, 0.85f, 0.85f} };
	gl_engine::Texture grey_tex{		glm::vec3{0.50f, 0.50f, 0.50f} };
	gl_engine::Texture dark_grey_tex{	glm::vec3{0.25f, 0.25f, 0.25f} };
	gl_engine::Texture blue_tex{		glm::vec3{0.25f, 0.25f, 1.00f} };
	gl_engine::Texture normal_up{		glm::vec3{0.50f, 0.50f, 1.00f} };

	// Textures
	gl_engine::Texture uv_template_b_tex{ "uvtemplateB.tga"  };
	gl_engine::Texture grey_grid_tex{ "greyGrid_01.tga" };
	gl_engine::Texture brick_normals{ "bricks2_normal.tga" };
	gl_engine::Texture brick_displacement{ "bricks2_disp.tga" };



	// Shader 1
	gl_engine::Material shaderBall_material{ "cMat", "cShader.vert", "cShader.frag" };
	shaderBall_material.set_sampler_value("material.glossiness", 0.5f);
	shaderBall_material.set_sampler_value("material.specular", 0.8f);
	shaderBall_material.set_texture("material.diffuse", &uv_template_b_tex);
	shaderBall_material.set_texture("material.normal", &brick_normals);
	shaderBall_material.set_uniform("material.normal_directx_mode", false);
	shaderBall_material.set_texture("material.displacement", &brick_displacement);
	shaderBall_material.set_uniform("material.displacement_amount", 0.06f);


	// Shader 2
	gl_engine::Material floor_material{ "floor material", "cShader.vert", "cShader.frag" };
	floor_material.set_sampler_value("material.glossiness", 0.03f);
	floor_material.set_sampler_value("material.specular", 0.30f);
	floor_material.set_texture("material.diffuse", &grey_grid_tex);
	floor_material.set_texture("material.normal", &normal_up);

	// Texture 3
	//gl_engine::Texture arrayTest_tex("uvtemplate.tga");
	//arrayTest_tex.set_target(GL_TEXTURE_2D_ARRAY);
	//shaderBall_material.set_texture("shadowMap", &arrayTest_tex);

	// Mesh 1
	gl_engine::Mesh shaderBall = gl_engine::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");
	gl_engine::MeshNode shaderBall_node{ "shader ball", &shaderBall, &shaderBall_material };

	// Mesh 2
	gl_engine::Mesh plane = gl_engine::Plane::create_plane(100.0f, 100.0f, 1, 1);
	plane.scale_uvs(10.0f);
	gl_engine::MeshNode plane_node{ "Plane 1", &plane, &floor_material };

	// Mesh 3
	//gl_engine::Mesh cube = gl_engine::Cube::create_cube(5.0f);
	//gl_engine::MeshNode cube_node{ "Cube 1", &cube, &cShadMat };
	//cube_node.set_parent(&freeCam_node);
	//cube_node.set_position({ 0.0f, 3.0f, 6.0f });


	// Point Light 1
	gl_engine::PointLight pointLight{ 1.0f, { 0.0f, 0.0f, 0.0f } };
	pointLight.set_brightness(4.1f);
	pointLight.set_color(glm::vec3(1.0, 0.7, 0.2));
	pointLight.set_radius(0.1f);
	//pointLight.disable_mesh();
	gl_engine::LightNode pointLight_node{ "Point Light 1", &pointLight };
	pointLight_node.set_position({ -4.0f, 1.2f, 0.0f });

	// Point Light 2
	gl_engine::PointLight pointLight2{ 9.0f, { 0.2f, 0.1f, 1.0f } };
	gl_engine::LightNode pointLight_node2{ "Point Light 2", &pointLight2 };
	pointLight_node2.set_position({ 0.0f, 4.0f, -5.0f });

	// Directional Light 1
	gl_engine::DirectionalLight directionalLight1{ 0.01f, {1.0f, 1.0f, 1.0f} /*{ 0.2f, 1.0f, 0.1f }*/ };
	gl_engine::LightNode directionalLight_node1{ "Directional Light 1", &directionalLight1 };
	directionalLight_node1.set_rotation({ 33.0f, 225.0f, 0.0f });
	directionalLight_node1.set_position({ 16.0f, 16.0f, 16.0f });

	directionalLight_node1.add_child(&freeCam_node);
	directionalLight_node1.add_child(&orthoCam_node);


	// Spot Light 1
	gl_engine::SpotLight spotLight1{ 10.0f, { 1.0f, 0.0f, 0.0f } };
	gl_engine::LightNode spotLight_node1{ "Spot Light 1", &spotLight1 };
	spotLight_node1.set_rotation({ 60.0f, 0.0f, 0.0f });
	spotLight_node1.set_position({ -3.0f, 10.0f, -2.0f });
	spotLight1.set_inner_angle(30.0f);
	spotLight1.set_outer_angle(33.0f);

	// Spot Light 2
	gl_engine::SpotLight spotLight2{ 14.0f, {0.0f, 0.0f, 1.0f } };
	gl_engine::LightNode spotLight_node2{ "Spot Light 2", &spotLight2 };
	spotLight_node2.set_position({ 6.0f, 12.0f, 2.0f });
	spotLight_node2.set_rotation({ 50.0f, -100.0f, 0.0f });
	spotLight2.set_inner_angle(25.0f);
	spotLight2.set_outer_angle(70.0f);

	// Shadow map
	gl_engine::ShadowMap shadowMap_spot1{ &spotLight_node1 };
	gl_engine::ShadowMap shadowMap_spot2{ &spotLight_node2 };
	gl_engine::ShadowMap shadowMap_directional1{ &directionalLight_node1 };
	gl_engine::ShadowMap shadowMap_point1{ &pointLight_node };
	gl_engine::ShadowMap shadowMap_point2{ &pointLight_node2 };

	shadowMap_point2.set_clip_far(25.0f);

	// Null node 1
	gl_engine::Node lightRotate1{ "light rotate 01" };
	//lightRotate1.add_child(&pointLight_node);

	// Null node 2
	gl_engine::Node lightRotate2{ "light rotate 02" };
	lightRotate2.add_child(&lightRotate1);
	lightRotate2.set_position({ 3.0, 0.0, 0.0 });

	// Axis arrows
	gl_engine::Mesh axis_arrow{ gl_engine::Arrow::create_arrow(20) };
	gl_engine::Material axis_mat_x{ "axis arrow x", "LightShader.vert", "LightShader.frag" };
	gl_engine::Material axis_mat_y{ "axis arrow x", "LightShader.vert", "LightShader.frag" };
	gl_engine::Material axis_mat_z{ "axis arrow x", "LightShader.vert", "LightShader.frag" };
	axis_mat_x.set_uniform("light.color", glm::vec3{ 1.0f, 0.0f, 0.0f });
	axis_mat_y.set_uniform("light.color", glm::vec3{ 0.0f, 1.0f, 0.0f });
	axis_mat_z.set_uniform("light.color", glm::vec3{ 0.0f, 0.0f, 1.0f });
	gl_engine::MeshNode axis_arrow_x_node{ "axis arrow x node", &axis_arrow, &axis_mat_x };
	gl_engine::MeshNode axis_arrow_y_node{ "axis arrow y node", &axis_arrow, &axis_mat_y };
	gl_engine::MeshNode axis_arrow_z_node{ "axis arrow z node", &axis_arrow, &axis_mat_z };
	axis_arrow_x_node.set_scale(0.05f);
	axis_arrow_y_node.set_scale(0.05f);
	axis_arrow_z_node.set_scale(0.05f);
	axis_arrow_x_node.set_position({ 1.0f, 0.0f, 0.0f });
	axis_arrow_y_node.set_position({ 0.0f, 1.0f, 0.05f });
	axis_arrow_z_node.set_position({ 0.0f, 0.0f, 1.0f });
	axis_arrow_x_node.set_rotation({ 0.0f, 90.0f, 0.0f });
	axis_arrow_y_node.set_rotation({ -90.0f, 0.0f, 0.0f });


	// Renderer
	gl_engine::Renderer render{ &targetCam_node, glm::uvec2(window.width(), window.height()) };
	render.add_node(&shaderBall_node);
	render.add_node(&plane_node);

	render.add_node(&pointLight_node);
	render.add_node(&pointLight_node2);
	//render.add_node(&directionalLight_node1);
	render.add_node(&spotLight_node1);
	render.add_node(&spotLight_node2);

	//render.add_node(&lightRotate1);


	//render.add_node(&axis_arrow_x_node);
	//render.add_node(&axis_arrow_y_node);
	//render.add_node(&axis_arrow_z_node);

	//uv_template_b_tex.unbind();

	gl_engine::Timer timer;

	while (render.poll_events())
	{
		float new_y_rotation = (float)shaderBall_node.local_rotation().y + (float)timer.delta_time_s() * 10.0f;
		shaderBall_node.set_rotation({ 0.0, new_y_rotation, 0.0 });

		float l1_rotate = lightRotate1.local_rotation().z - (float)timer.delta_time_s() * 10;
		lightRotate1.set_rotation({ 0.0 , 0.0, l1_rotate });

		float l2_rotate = lightRotate2.local_rotation().y - (float)timer.delta_time_s() * 60;
		lightRotate2.set_rotation({ 0.0, l2_rotate, 0.0 });

		render.update(&window, &timer);
	}
}

void three_shader_ball_scene(gl_engine::Window window)
{
	GLfloat shader_ball_scale = 0.6f;
	GLfloat shader_ball_offset = 3.5f;
	glm::vec3 shader_ball_rotation{ 0.0f, -35.0f, 0.0f };

	// Target Camera
	gl_engine::TargetCamera targetCam{};
	gl_engine::CameraNode targetCam_node{ "Target Camera 1", &targetCam };
	targetCam.set_position({ 0.0f, 8.0f, 8.0f });
	targetCam.set_focus_target({ 0.0f, 0.0f, 0.0f });
	targetCam.focus(glm::vec3{ 0.0f, 0.0f, 0.0f });
	targetCam.set_clip_far(1000.0f);

	// Colors
	gl_engine::Texture white_tex(		glm::vec3(1.00f, 1.00f, 1.00f));
	gl_engine::Texture lighter_grey_tex(glm::vec3(0.95f, 0.95f, 0.95f));
	gl_engine::Texture light_grey_tex(	glm::vec3(0.85f, 0.85f, 0.85f));
	gl_engine::Texture grey_tex(		glm::vec3(0.50f, 0.50f, 0.50f));
	gl_engine::Texture dark_grey_tex(	glm::vec3(0.25f, 0.25f, 0.25f));
	gl_engine::Texture darker_grey_tex(	glm::vec3(0.10f, 0.10f, 0.10f));
	gl_engine::Texture red_tex{			glm::vec3{1.00f, 0.00f, 0.00f} };
	gl_engine::Texture green_tex{		glm::vec3{0.00f, 1.00f, 0.00f} };
	gl_engine::Texture blue_tex{		glm::vec3{0.00f, 0.00f, 1.00f} };
	gl_engine::Texture normal_up{		glm::vec3{0.50f, 0.50f, 1.00f} };
	gl_engine::Texture grid_tex{			"greyGrid_01.tga"		};
	gl_engine::Texture brick_normals{		"bricks2_normal.tga"	};
	gl_engine::Texture brick_displacement{	"bricks2_disp.tga"		};



	// Shader Grey
	gl_engine::Material grey_material_rough{		"grey material rough",	"cShader.vert", "cShader.frag" };
	gl_engine::Material grey_material_mid{			"grey material mid",	"cShader.vert", "cShader.frag" };
	gl_engine::Material grey_material_shiny{		"grey material shiny",	"cShader.vert", "cShader.frag" };
	gl_engine::Material grey_material_rough_mapped{	"grey material rough",	"cShader.vert", "cShader.frag" };
	gl_engine::Material grey_material_mid_mapped{	"grey material mid",	"cShader.vert", "cShader.frag" };
	gl_engine::Material grey_material_shiny_mapped{	"grey material shiny",	"cShader.vert", "cShader.frag" };

	grey_material_rough.set_sampler_value(		"material.glossiness", 0.01f);
	grey_material_mid.set_sampler_value(		"material.glossiness", 0.50f);
	grey_material_shiny.set_sampler_value(		"material.glossiness", 1.00f);
	grey_material_rough_mapped.set_sampler_value("material.glossiness", 0.01f);
	grey_material_mid_mapped.set_sampler_value(	"material.glossiness", 0.50f);
	grey_material_shiny_mapped.set_sampler_value("material.glossiness", 1.00f);

	grey_material_rough.set_sampler_value(			"material.specular", 1.0f);
	grey_material_mid.set_sampler_value(			"material.specular", 1.0f);
	grey_material_shiny.set_sampler_value(			"material.specular", 1.0f);
	grey_material_rough_mapped.set_sampler_value(	"material.specular", 1.0f);
	grey_material_mid_mapped.set_sampler_value(		"material.specular", 1.0f);
	grey_material_shiny_mapped.set_sampler_value(	"material.specular", 1.0f);

	grey_material_rough.set_texture(		"material.diffuse", &grid_tex);
	grey_material_mid.set_texture(			"material.diffuse", &grid_tex);
	grey_material_shiny.set_texture(		"material.diffuse", &grid_tex);
	grey_material_rough_mapped.set_texture(	"material.diffuse", &grid_tex);
	grey_material_mid_mapped.set_texture(	"material.diffuse", &grid_tex);
	grey_material_shiny_mapped.set_texture(	"material.diffuse", &grid_tex);

	grey_material_rough.set_texture(		"material.normal", &brick_normals);
	grey_material_mid.set_texture(			"material.normal", &brick_normals);
	grey_material_shiny.set_texture(		"material.normal", &brick_normals);
	grey_material_rough_mapped.set_texture(	"material.normal", &brick_normals);
	grey_material_mid_mapped.set_texture(	"material.normal", &brick_normals);
	grey_material_shiny_mapped.set_texture(	"material.normal", &brick_normals);

	grey_material_rough_mapped.set_texture(	"material.displacement", &brick_displacement);
	grey_material_mid_mapped.set_texture(	"material.displacement", &brick_displacement);
	grey_material_shiny_mapped.set_texture(	"material.displacement", &brick_displacement);

	grey_material_rough_mapped.set_uniform(	"material.displacement_amount", 0.03f);
	grey_material_mid_mapped.set_uniform(	"material.displacement_amount", 0.03f);
	grey_material_shiny_mapped.set_uniform(	"material.displacement_amount", 0.03f);

	// Shader ball mesh
	gl_engine::Mesh shaderBall = gl_engine::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");
	//gl_engine::Mesh shaderBall = gl_engine::Sphere::create_sphere(2.5f);

	// Spec test shader ball nodes
	gl_engine::MeshNode shaderBall_rough_node{		"shaderBall rough",			&shaderBall, &grey_material_rough };
	gl_engine::MeshNode shaderBall_mid_node{		"shaderBall mid",			&shaderBall, &grey_material_mid };
	gl_engine::MeshNode shaderBall_shiny_node{		"shaderBall shiny",			&shaderBall, &grey_material_shiny };
	gl_engine::MeshNode shaderBall_rough_mapped_node{"shaderBall blinn rough",	&shaderBall, &grey_material_rough_mapped };
	gl_engine::MeshNode shaderBall_mid_mapped_node{  "shaderBall blinn mid",		&shaderBall, &grey_material_mid_mapped };
	gl_engine::MeshNode shaderBall_shiny_mapped_node{"shaderBall blinn shiny",	&shaderBall, &grey_material_shiny_mapped };

	std::vector<gl_engine::MeshNode*> shaderBalls;
	shaderBalls.push_back(&shaderBall_rough_node);
	shaderBalls.push_back(&shaderBall_mid_node);
	shaderBalls.push_back(&shaderBall_shiny_node);
	shaderBalls.push_back(&shaderBall_rough_mapped_node);
	shaderBalls.push_back(&shaderBall_mid_mapped_node);
	shaderBalls.push_back(&shaderBall_shiny_mapped_node);

	for (auto & shad_ball : shaderBalls)
	{
		shad_ball->set_scale(shader_ball_scale);
		shad_ball->set_rotation(shader_ball_rotation);
	}

	// Position shader balls
	shaderBall_rough_node.set_position({ -shader_ball_offset, shader_ball_offset / 2, 0.0f });
	shaderBall_mid_node.set_position({ 0.0f, shader_ball_offset / 2, 0.0f });
	shaderBall_shiny_node.set_position({ shader_ball_offset, shader_ball_offset / 2, 0.0f });
	shaderBall_rough_mapped_node.set_position({ -shader_ball_offset, -shader_ball_offset / 2, 0.0f });
	shaderBall_mid_mapped_node.set_position({ 0.0f, -shader_ball_offset / 2, 0.0f });
	shaderBall_shiny_mapped_node.set_position({ shader_ball_offset, -shader_ball_offset / 2, 0.0f });

	// Directional light 1
	gl_engine::DirectionalLight directionalLight1{ 0.2f, {1.0f, 1.0f, 1.0f} /*{ 0.2f, 1.0f, 0.1f }*/ };
	gl_engine::LightNode directionalLight_node1{ "Directional Light 1", &directionalLight1 };
	directionalLight_node1.set_rotation({ 33.0f, 225.0f, 0.0f });
	directionalLight_node1.set_position({ 16.0f, 16.0f, 16.0f });

	// Shadow map
	//gl_engine::ShadowMap shadowMap_directional1{ &directionalLight_node1 };


	// Renderer
	gl_engine::Renderer render{ &targetCam_node, glm::uvec2(window.width(), window.height()) };

	render.add_node(&shaderBall_rough_node);
	render.add_node(&shaderBall_mid_node);
	render.add_node(&shaderBall_shiny_node);
	render.add_node(&shaderBall_rough_mapped_node);
	render.add_node(&shaderBall_mid_mapped_node);
	render.add_node(&shaderBall_shiny_mapped_node);

	render.add_node(&directionalLight_node1);

	gl_engine::Timer timer;

	while (render.poll_events())
	{
		render.update(&window, &timer);

		//shaderBall_rough_node.set_position(glm::vec3{ std::cos(timer.total_time_s()) * 5.0f, 0.0f, 0.0f });
	}
}