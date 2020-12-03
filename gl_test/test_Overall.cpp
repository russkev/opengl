#include "pch.h"
#include "CppUnitTest.h"

#include "gl_engine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gl_test
{
	TEST_CLASS(Overall)
	{
		glen::Window window{ "Window", 1u, 1u };

	public:
		TEST_METHOD(Construct_Window)
		{
			GLuint width = 1u, height = 1u;
			const glm::uvec2 dimensions{ width, height };
			glen::Window window{ "Window", width, height };

			Assert::IsTrue(true);
		}

		TEST_METHOD(Construct_Meshes)
		{
			glen::Mesh m_cube{ glen::Cube::create_cube() };
			glen::Mesh arrow{ glen::Arrow::create_arrow() };
			glen::Mesh cylinder{ glen::Cylinder::create_cylinder() };
			glen::Mesh plane{ glen::Plane::create_plane() };
			glen::Mesh sphere{ glen::Sphere::create_sphere(2.0f) };
			glen::Mesh window_quad{ glen::WindowQuad::create_windowQuad() };
			//glen::Mesh shader_ball{ glen::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj") };
			
			Assert::IsTrue(true);

		}

		TEST_METHOD(Construct_Cameras)
		{
			glen::FreeCamera free_camera{};
			glen::OrthoCamera ortho_camera{};
			glen::TargetCamera target_camera{};

			Assert::IsTrue(true);
		}

		TEST_METHOD(Construct_Lights)
		{
			glen::DirectionalLight directional_light{ 1.0f, glm::vec3{1.0f} };
			glen::PointLight point_light{ 1.0f, glm::vec3{1.0f} };
			glen::SpotLight spot_light{ 1.0f, glm::vec3{1.0f} };

			Assert::IsTrue(true);
		}

		TEST_METHOD(Construct_Materials)
		{
			glen::BlinnMaterial blinn_material{};
			glen::BlinnDeferredMaterial blinn_deferred_material{};
			glen::BloomMaterial bloom_material{};
			glen::DepthCubeMaterial depth_cube_material{};
			glen::GaussianBlurMaterial gaussian_blur_material{};
			glen::HDRMaterial hdr_material{};
			glen::LightMaterial light_material{};

			Assert::IsTrue(true);
		}

		TEST_METHOD(Construct_Nodes)
		{
			glen::FreeCamera free_camera{};
			glen::PointLight point_light{ 1.0f, glm::vec3{1.0f} };
			glen::Mesh cube{ glen::Cube::create_cube() };
			glen::BlinnMaterial blinn_material{};

			glen::Node node{ "Node" };
			glen::CameraNode camera_node{ "Camera Node", &free_camera };
			glen::LightNode light_node{ "Light Node", &point_light };
			glen::MeshNode mesh_node{ "Mesh Node", &cube, &blinn_material };

			Assert::IsTrue(true);
		}

		TEST_METHOD(Construct_Shading_Elements)
		{
			glen::PointLight point_light{ 1.0f, glm::vec3{1.0f} };
			glen::LightNode light_node{ "Light Node", &point_light };


			glen::Framebuffer framebuffer{ GL_TEXTURE_2D };
			glen::ShadowMap shadow_map{ &light_node };
			glen::Text2D grid_tex{ (glen::Material::shaders_dir() + "..\\textures\\fonts\\font_calibri.tga").c_str() };
			glen::Texture texture{ GL_TEXTURE_2D };

			Assert::IsTrue(true);
		}

		TEST_METHOD(Construct_Post_Effects)
		{
			glen::Framebuffer framebuffer{ GL_TEXTURE_2D };
			glm::uvec2 dimensions{ 800u, 600u };

			glen::ToneMap tone_map{ &framebuffer, dimensions };
			glen::Bloom bloom{ dimensions, &tone_map };

			Assert::IsTrue(true);
		}

		TEST_METHOD(Construct_Timer)
		{
			glen::Timer timer{ 1.0f };

			Assert::IsTrue(true);
		}

		TEST_METHOD(Construct_Render_Elements)
		{
			glm::uvec2 dimensions{ 800u, 600u };
			glen::FreeCamera free_camera{};
			glen::CameraNode camera_node{ "Camera Node", &free_camera };
			glen::Framebuffer framebuffer{ GL_TEXTURE_2D };
			glen::LightMaterial light_material{};


			glen::Deferred{ GL_TEXTURE_2D, &framebuffer, &light_material, dimensions };
			glen::Renderer{ &camera_node, dimensions };
			
			Assert::IsTrue(true);
		}
	};
}