#ifndef GL_DEMO_SHADER_BALL_HD
#define GL_DEMO_SHADER_BALL_HD

#include "gl_engine.h"

namespace gl_demo
{
	static void shader_ball_hd_demo()
	{
		GLuint width = 800u;
		GLuint height = 600u;

		glen::Window window{ "GL_Engine", width, height };

		// Target Camera
		glen::TargetCamera target_cam{};
		glen::CameraNode target_cam_node{ "Target Camera 1", &target_cam };
		target_cam.set_position({ 0.0f, 320.0f, 600.0f });
		target_cam.set_focus_target({ 0.0f, 110.0f, 0.0f });
		target_cam.focus(glm::vec3{ 0.0f, 110.0f, 0.0f });
		target_cam.set_clip_far(3000.0f);
		target_cam.set_angle_of_view(37.8f); //35mm


		// Meshes
		glen::Mesh ball_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_ball.obj");
		glen::Mesh ball_inside_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_ball_inside.obj");
		glen::Mesh ball_trim_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_ball_trim.obj");
		glen::Mesh base_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_base.obj");
		glen::Mesh cyc_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_cyc.obj");

		// Materials
		glen::BlinnMaterial white_blinn_material{ "Blinn Material " };
		white_blinn_material.set_sampler_value(white_blinn_material.k_material_diffuse, 0.8f);
		white_blinn_material.set_uniform(white_blinn_material.k_material_specular_amount, 0.8f);
		white_blinn_material.set_sampler_value(white_blinn_material.k_material_specular, 0.9f);
		white_blinn_material.set_uniform("directionalLight[1].shadow_enabled", true);


		// Mesh Nodes
		glen::MeshNode ball_node{ "Ball Node", &ball_mesh, &white_blinn_material };
		glen::MeshNode ball_inside_node{ "Ball Inside Node", &ball_inside_mesh, &white_blinn_material };
		glen::MeshNode ball_trim_node{ "Ball Trim Node", &ball_trim_mesh, &white_blinn_material };
		glen::MeshNode base_node{ "Base Node", &base_mesh, &white_blinn_material };
		glen::MeshNode cyc_node{ "Cyc Node", &cyc_mesh, &white_blinn_material };

		// Directional Light 1
		glen::SpotLight spot_light{ 70.0f, {1.0f, 1.0f, 1.0f} /*{ 0.2f, 1.0f, 0.1f }*/ };
		spot_light.camera()->set_clip_far(2000.0f);
		spot_light.set_inner_angle(50.0f);
		spot_light.set_outer_angle(90.0f);
		glen::LightNode spot_light_node{ "Spot Light 1", &spot_light };
		spot_light_node.set_rotation({ 33.0f, 270.0f, 0.0f });
		spot_light_node.set_position({ 300.0f, 300.0f, 0.0f });

		// Point Light 1
		glen::PointLight point_light{ 80.0f, glm::vec3{0.0f, 1.0f, 0.0f} };
		point_light.camera()->set_clip_far(2000.0f);
		point_light.camera()->set_clip_near(10.0f);
		glen::LightNode point_light_node{ "Point Light 1", &point_light };
		point_light_node.set_position(glm::vec3{ -300.0f, 300.0f, 0.0f });

		// Shadow Map
		glen::ShadowMap spot_light_shadow{ &spot_light_node };
		spot_light_shadow.set_clip_near(1.0f);
		spot_light_shadow.set_clip_far(2000.0f);
		glen::ShadowMap point_light_shadow{ &point_light_node };

		glen::Renderer render{ &target_cam_node, glm::uvec2{ window.width(), window.height() } };
		render.enable_post_effects();
		render.disable_deferred_render();
		render.disable_ao();

		// Add mesh nodes
		render.add_node(&ball_node);
		render.add_node(&ball_inside_node);
		render.add_node(&ball_trim_node);
		render.add_node(&base_node);
		render.add_node(&cyc_node);

		// Add light nodes
		render.add_node(&spot_light_node);
		render.add_node(&point_light_node);

		glen::Timer timer;
		while (render.poll_events())
		{
			render.update(&window, &timer);
		}

	}
}

#endif
