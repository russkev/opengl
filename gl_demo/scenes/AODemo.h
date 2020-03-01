#ifndef GL_DEMO_AO_H
#define GL_DEMO_AO_H

#include "gl_engine.h"

namespace gl_demo
{
	static void ao_demo()
	{
		GLuint width  = 800u;
		GLuint height = 600u;

		glen::Window window{ "GL_Engine", width, height };

		// Target Camera
		glen::TargetCamera target_cam{};
		glen::CameraNode target_cam_node{ "Target Camera 1", &target_cam };
		target_cam.set_position({ 0.0f, 8.0f, 8.0f });
		target_cam.set_focus_target({ 0.0f, 0.0f, 0.0f });
		target_cam.focus(glm::vec3{ 0.0f, 0.0f, 0.0f });
		target_cam.set_clip_far(1000.0f);

		// Shader Ball Mesh
		glen::Mesh shader_ball = glen::OBJ_Loader::load_obj("shaderball_lowpoly_02_tris.obj");

		// Plane Mesh
		glen::Mesh plane = glen::Plane::create_plane(100.0f, 100.0f, 1, 1);
		plane.scale_uvs(10.0f);

		// Material 
		glen::LightMaterial white_material{ "White Material" };
		white_material.set_uniform(white_material.k_light_color, glm::vec3{ 0.8f });
		white_material.set_uniform(white_material.k_light_brightness, 1.0f);


		glen::BlinnMaterial blinn_material{ "Blinn Material " };
		blinn_material.set_sampler_value(blinn_material.k_color, 0.8f);

		glen::AO_GBufferMaterial ao_g_buffer_material{};

		//ao_g_buffer_material.set_sampler_color(ao_g_buffer_material.k_g_diffuse, glm::vec3{ 0.2f, 0.9f, 0.2f });

		// Mesh Nodes
		glen::MeshNode shader_ball_node{ "Shader Ball Node", &shader_ball,  &blinn_material };
		glen::MeshNode floor_node{ "Floor Node", &plane, &blinn_material };

		// Point Light 1
		glen::PointLight point_light{ 1.0f, { 0.0f, 0.0f, 0.0f } };
		point_light.set_brightness(4.1f);
		point_light.set_color(glm::vec3(1.0, 0.7, 0.2));
		point_light.set_radius(0.1f);
		glen::LightNode point_light_node{ "Point Light 1", &point_light };
		point_light_node.set_position({ -4.0f, 1.2f, 0.0f });

		// Directional Light 1
		glen::DirectionalLight directional_light{ 0.7f, {1.0f, 1.0f, 1.0f} /*{ 0.2f, 1.0f, 0.1f }*/ };
		glen::LightNode directional_light_node{ "Directional Light 1", &directional_light };
		directional_light_node.set_rotation({ 33.0f, 225.0f, 0.0f });
		directional_light_node.set_position({ 16.0f, 16.0f, 16.0f });

		glen::ShadowMap directional_light_shadow{ &directional_light_node };

		// Renderer
		glen::Renderer render{ &target_cam_node, glm::uvec2{ window.width(), window.height() } };
		render.disable_post_effects();
		render.disable_deferred_render();
		//render.enable_deferred_render();
		render.enable_ao();
		//render.disable_ao();

		render.add_node(&shader_ball_node);
		render.add_node(&floor_node);

		render.add_node(&point_light_node);
		render.add_node(&directional_light_node);

		glen::Timer timer;

		while (render.poll_events())
		{
			render.update(&window, &timer);
		}
	}
}


#endif

