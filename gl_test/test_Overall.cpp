#include "pch.h"
#include <boost/test/unit_test.hpp>

#include "gl_engine.h"

BOOST_AUTO_TEST_SUITE(Overall)

BOOST_AUTO_TEST_CASE(Construct_everything)
{
	GLuint width = 800u, height = 600u;
	const glm::uvec2 dimensions{ width, height };
	glen::Window window{ "Window", width, height };

	glen::Mesh cube{ glen::Cube::create_cube() };
	glen::Mesh arrow{ glen::Arrow::create_arrow() };
	glen::Mesh cylinder{ glen::Cylinder::create_cylinder() };
	glen::Mesh shader_ball{ glen::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj") };
	glen::Mesh plane{ glen::Plane::create_plane() };
	glen::Mesh sphere{ glen::Sphere::create_sphere(2.0f) };
	glen::Mesh window_quad{ glen::WindowQuad::create_windowQuad() };

	glen::FreeCamera free_camera{};
	glen::OrthoCamera ortho_camera{};
	glen::TargetCamera target_camera{};

	glen::DirectionalLight directional_light{ 1.0f, glm::vec3{1.0f} };
	glen::PointLight point_light{ 1.0f, glm::vec3{1.0f} };
	glen::SpotLight spot_light{ 1.0f, glm::vec3{1.0f} };

	glen::BlinnMaterial blinn_material{};
	glen::BlinnDeferredMaterial blinn_deferred_material{};
	glen::BloomMaterial bloom_material{};
	glen::DepthCubeMaterial depth_cube_material{};
	glen::GaussianBlurMaterial gaussian_blur_material{};
	glen::HDRMaterial hdr_material{};
	glen::LightMaterial light_material{};

	glen::CameraNode camera_node{ "Camera Node", &free_camera };
	glen::LightNode light_node{ "Light Node", &point_light };
	glen::MeshNode mesh_node{ "Mesh Node", &cube, &blinn_material };

	glen::Framebuffer framebuffer{ GL_TEXTURE_2D };
	glen::ShadowMap shadow_map{ &light_node };
	glen::Text2D text_2d{ "font_calibri_01.tga" };
	glen::Texture texture{ GL_TEXTURE_2D };

	glen::ToneMap tone_map{ &framebuffer, &dimensions };
	glen::Bloom bloom{ &framebuffer, &dimensions, &tone_map };

	glen::Timer timer{ 1.0f };

	glen::DeferredRender{ GL_TEXTURE_2D, dimensions };
	glen::Renderer{ &camera_node, dimensions };

	BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()