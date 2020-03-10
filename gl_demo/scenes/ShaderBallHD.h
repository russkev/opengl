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
		glm::vec3 centre_focus{ 0.0f, 110.0f, 0.0f };
		target_cam.set_position({ 0.0f, 320.0f, 600.0f });
		target_cam.set_focus_target(centre_focus);
		target_cam.focus(centre_focus);
		target_cam.set_clip_far(6000.0f);
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
		white_blinn_material.set_sampler_value(white_blinn_material.k_material_glossiness, 0.5f);
		white_blinn_material.set_uniform(white_blinn_material.k_ambient_light_brightness, 0.2f);

		// Mesh Nodes
		glen::MeshNode ball_node{ "Ball Node", &ball_mesh, &white_blinn_material };
		glen::MeshNode ball_inside_node{ "Ball Inside Node", &ball_inside_mesh, &white_blinn_material };
		glen::MeshNode ball_trim_node{ "Ball Trim Node", &ball_trim_mesh, &white_blinn_material };
		glen::MeshNode base_node{ "Base Node", &base_mesh, &white_blinn_material };
		glen::MeshNode cyc_node{ "Cyc Node", &cyc_mesh, &white_blinn_material };

		// Spot Light Key 1
		glen::SpotLight spot_light_key{ 140.0f, {1.0f, 1.0f, 1.0f} };
		spot_light_key.set_brightness(200.0f);
		spot_light_key.set_color(glm::vec3(0.6, 0.7, 1.0));
		spot_light_key.set_inner_angle(20.0f);
		spot_light_key.set_outer_angle(45.0f);

		glen::LightNode spot_light_key_node{ "Spot Light Key 1", &spot_light_key };
		spot_light_key_node.set_rotation({ 50.0f, 240.0f, 0.0f });
		spot_light_key_node.set_position({ 300.0f, 500.0f, 100.0f });

		glen::ShadowMap spot_light_key_shadow{ &spot_light_key_node, 4096 };
		spot_light_key_shadow.set_bias(0.000006f);
		spot_light_key_shadow.set_radius(2.0f);
		spot_light_key_shadow.set_num_samples(5);
		spot_light_key_shadow.set_clip_far(4000);

		// Spot Light Fill 1
		glen::SpotLight spot_light_fill{ 0.0f, {0.0f, 0.0f, 0.0f} };
		spot_light_fill.set_brightness(100.0f);
		spot_light_fill.set_color(glm::vec3(1.0f, 0.85f, 0.6f));
		spot_light_fill.set_inner_angle(20.0f);
		spot_light_fill.set_outer_angle(45.0f);

		glen::LightNode spot_light_fill_node{ "Spot Light Fill 1", &spot_light_fill };
		spot_light_fill_node.set_rotation({ 40.0f, 120.0f, 0.0f });
		spot_light_fill_node.set_position({ -300.0f, 500.0f, 200.0f });

		glen::ShadowMap spot_light_fill_shadow{ &spot_light_fill_node, 4096 };
		spot_light_fill_shadow.set_bias(0.000006f);
		spot_light_fill_shadow.set_radius(2.0f);
		spot_light_fill_shadow.set_num_samples(5);
		spot_light_fill_shadow.set_clip_far(4000);

		// Point Light Fill 1
		glen::PointLight point_light_fill{0.0, {0.0f, 0.0f, 0.0f} };
		point_light_fill.set_brightness(0.5f);
		point_light_fill.set_color(glm::vec3{ 0.0f, 1.0f, 0.0f });
		point_light_fill.set_radius(10.0f);
		
		glen::LightNode point_light_fill_node{ "Point Light Fill 1", &point_light_fill };
		point_light_fill_node.set_position({ 0.0f, 80.0f, 100.0f });



		// Spot Light BG 1
		glen::SpotLight spot_light_bg_01{ 140.0f, {1.0f, 1.0f, 1.0f} };
		spot_light_bg_01.set_brightness(200.0f);
		spot_light_bg_01.set_color(glm::vec3(0.6f, 0.7f, 1.0f));
		spot_light_bg_01.set_inner_angle(5.0f);
		spot_light_bg_01.set_outer_angle(60.0f);

		glen::LightNode spot_light_bg_01_node{ "Spot Light BG 1", &spot_light_bg_01 };
		spot_light_bg_01_node.set_rotation({ 25.0f, 200.0f, 0.0f });
		spot_light_bg_01_node.set_position({ -200.0f, 600.0f, -100.0f });

		// Spot Light BG 2
		glen::SpotLight spot_light_bg_02{ 140.0f, {1.0f, 1.0f, 1.0f} };
		spot_light_bg_02.set_brightness(150.0f);
		spot_light_bg_02.set_color(glm::vec3(1.0f, 0.9f, 0.8f));
		spot_light_bg_02.set_inner_angle(1.0f);
		spot_light_bg_02.set_outer_angle(60.0f);

		glen::LightNode spot_light_bg_02_node{ "Spot Light BG 2", &spot_light_bg_02 };
		spot_light_bg_02_node.set_rotation({ 35.0f, 140.0f, 0.0f });
		spot_light_bg_02_node.set_position({ 300.0f, 400.0f, -100.0f });



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
		render.add_node(&spot_light_key_node);
		render.add_node(&spot_light_fill_node);
		render.add_node(&point_light_fill_node);
		render.add_node(&spot_light_bg_01_node);
		render.add_node(&spot_light_bg_02_node);

		glen::Timer timer;
		while (render.poll_events())
		{
			point_light_fill.set_brightness(2.0f);
			point_light_fill.set_color(glm::vec3{ 1.0f, 1.0f, 1.0f });
			point_light_fill.set_radius(10.0f);
			point_light_fill.disable_mesh();

			point_light_fill_node.set_position({ 60.0f, 35.0f, 120.0f });


			render.update(&window, &timer);
		}

	}
}
#endif
