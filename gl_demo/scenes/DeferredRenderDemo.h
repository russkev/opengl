#ifndef GL_DEMO_DEFERRED_RENDER_H
#define GL_DEMO_DEFERRED_RENDER_H

#include "gl_engine.h"


namespace gl_demo
{
	static void deferred_demo()
	{
		GLuint width = 800u;
		GLuint height = 600u;

		glen::Window window{ "GL Engine", width, height };

		// Target Camera
		glen::TargetCamera targetCam{};
		glen::CameraNode targetCam_node{ "Target Camera 1", &targetCam };
		targetCam.set_position({ 0.0f, 8.0f, 8.0f });
		targetCam.set_focus_target({ 0.0f, 0.0f, 0.0f });
		targetCam.focus(glm::vec3{ 0.0f, 0.0f, 0.0f });
		targetCam.set_clip_far(1000.0f);

		// Shader ball mesh
		glen::Mesh shaderBall = glen::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");

		// GBuffer material
		//gl_engine::Material g_buffer_material{ "G Buffer Material", "GBuffer.vert", "GBuffer.frag" };
		glen::GBufferMaterial g_buffer_material{};

		//gl_engine::Material test_material{ "Test material", "Blinn.vert", "Blinn.frag" };

		// Textures
		glen::Texture diffuse_texture{ "uvtemplate.tga" };
		glen::Texture spec_texture{ "greyGrid_01.tga" };

		g_buffer_material.set_texture(glen::GBufferMaterial::k_material_diffuse, &diffuse_texture);
		g_buffer_material.set_texture(glen::GBufferMaterial::k_material_specular, &spec_texture);

		// Mesh node
		glen::MeshNode g_buffer_node{ "G Buffer Mesh Node", &shaderBall, &g_buffer_material };

		// Directional light 1
		glen::DirectionalLight directionalLight1{ 0.8f, {1.0f, 0.4f, 1.0f} };
		glen::LightNode directionalLight_node1{ "Directional Light 1", &directionalLight1 };
		directionalLight_node1.set_rotation({ 33.0f, 225.0f, 0.0f });
		directionalLight_node1.set_position({ 16.0f, 16.0f, 16.0f });

		// Point Light 1
		glen::PointLight pointLight{ 1.0f, { 0.0f, 0.0f, 0.0f } };
		pointLight.set_brightness(4.1f);
		pointLight.set_color(glm::vec3(1.0, 0.7, 0.2));
		pointLight.set_radius(0.1f);
		glen::LightNode pointLight_node{ "Point Light 1", &pointLight };
		pointLight_node.set_position({ -4.0f, 1.2f, 0.0f });

		// Renderer
		glen::Renderer render{ &targetCam_node, glm::uvec2(window.width(), window.height()) };
		render.disable_post_effects();

		render.enable_deferred_render();
		render.disable_ao();
		
		//render.disable_deferred_render();
		//render.enable_ao();

		render.add_node(&g_buffer_node);
		render.add_node(&directionalLight_node1);
		render.add_node(&pointLight_node);
		render.disable_post_effects();

		glen::Timer timer;

		while (render.poll_events())
		{
			render.update(&window, &timer);
		}

	}
}
#endif