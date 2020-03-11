#ifndef GL_DEMO_SHADER_BALL_HD
#define GL_DEMO_SHADER_BALL_HD

#include "gl_engine.h"

namespace gl_demo
{
	struct ShaderBallHD
	{
		static constexpr GLuint k_width = 800u;
		static constexpr GLuint k_height = 600u;


	public:
		ShaderBallHD()
		{
			glm::vec3 centre_focus{ 0.0f, 110.0f, 0.0f };
			m_target_cam.set_position({ 0.0f, 320.0f, 600.0f });
			m_target_cam.set_focus_target(centre_focus);
			m_target_cam.focus(centre_focus);
			m_target_cam.set_clip_far(6000.0f);
			m_target_cam.set_angle_of_view(37.8f); //35mm

			m_white_blinn_material.set_sampler_value(m_white_blinn_material.k_material_diffuse, 0.8f);
			m_white_blinn_material.set_uniform(m_white_blinn_material.k_material_specular_amount, 0.8f);
			m_white_blinn_material.set_sampler_value(m_white_blinn_material.k_material_specular, 0.9f);
			m_white_blinn_material.set_sampler_value(m_white_blinn_material.k_material_glossiness, 0.5f);
			m_white_blinn_material.set_uniform(m_white_blinn_material.k_ambient_light_brightness, 0.2f);

			m_white_diffuse_material.set_sampler_value(m_white_diffuse_material.k_material_diffuse, 0.98f);

			m_rubber_material.set_sampler_value(m_rubber_material.k_material_diffuse, 0.1f);
			m_rubber_material.set_uniform(m_rubber_material.k_material_specular_amount, 0.8f);
			m_rubber_material.set_sampler_value(m_rubber_material.k_material_specular, 0.9f);
			m_rubber_material.set_sampler_value(m_rubber_material.k_material_glossiness, 0.9f);

			m_grid_material.set_texture(m_grid_material.k_material_diffuse, &m_grid_texture);

			m_spot_light_key.set_brightness(200.0f);
			m_spot_light_key.set_color(glm::vec3(0.6, 0.7, 1.0));
			m_spot_light_key.set_inner_angle(20.0f);
			m_spot_light_key.set_outer_angle(45.0f);

			m_spot_light_key_node.set_rotation({ 50.0f, 240.0f, 0.0f });
			m_spot_light_key_node.set_position({ 300.0f, 500.0f, 100.0f });

			m_spot_light_key_shadow.set_bias(0.000006f);
			m_spot_light_key_shadow.set_radius(2.0f);
			m_spot_light_key_shadow.set_num_samples(5);
			m_spot_light_key_shadow.set_clip_far(4000);

			m_spot_light_fill.set_brightness(100.0f);
			m_spot_light_fill.set_color(glm::vec3(1.0f, 0.85f, 0.6f));
			m_spot_light_fill.set_inner_angle(20.0f);
			m_spot_light_fill.set_outer_angle(45.0f);

			m_spot_light_fill_node.set_rotation({ 40.0f, 120.0f, 0.0f });
			m_spot_light_fill_node.set_position({ -400.0f, 500.0f, 200.0f });

			m_spot_light_fill_shadow.set_bias(0.00006f);
			m_spot_light_fill_shadow.set_radius(20.0f);
			m_spot_light_fill_shadow.set_num_samples(5);
			m_spot_light_fill_shadow.set_clip_far(4000);

			m_point_light_fill.set_brightness(2.0f);
			m_point_light_fill.set_color(glm::vec3{ 1.0f, 1.0f, 1.0f });
			m_point_light_fill.set_radius(10.0f);
			m_point_light_fill.disable_mesh();
			m_point_light_fill.disable_specular();

			m_point_light_fill_node.set_position({ 60.0f, 35.0f, 120.0f });

			m_point_light_fill_2.set_brightness(1.0f);
			m_point_light_fill_2.set_color(glm::vec3{ 1.0f, 1.0f, 1.0f });
			m_point_light_fill_2.set_radius(10.0f);
			m_point_light_fill_2.disable_mesh();
			m_point_light_fill_2.disable_specular();

			m_point_light_fill_2_node.set_position({ 0.0f, 50.0f, 0.0f });

			m_spot_light_bg_01.set_brightness(200.0f);
			m_spot_light_bg_01.set_color(glm::vec3(0.6f, 0.7f, 1.0f));
			m_spot_light_bg_01.set_inner_angle(5.0f);
			m_spot_light_bg_01.set_outer_angle(60.0f);

			m_spot_light_bg_01_node.set_rotation({ 25.0f, 200.0f, 0.0f });
			m_spot_light_bg_01_node.set_position({ -200.0f, 600.0f, -100.0f });

			m_spot_light_bg_02.set_brightness(150.0f);
			m_spot_light_bg_02.set_color(glm::vec3(1.0f, 0.9f, 0.8f));
			m_spot_light_bg_02.set_inner_angle(1.0f);
			m_spot_light_bg_02.set_outer_angle(60.0f);

			m_spot_light_bg_02_node.set_rotation({ 35.0f, 140.0f, 0.0f });
			m_spot_light_bg_02_node.set_position({ 300.0f, 400.0f, -100.0f });
		}

		void add_member_nodes_to_renderer(glen::Renderer& render)
		{
			render.disable_post_effects();
			render.disable_deferred_render();
			render.enable_ao();

			// Add mesh nodes
			render.add_node(&m_ball_node);
			render.add_node(&m_ball_inside_node);
			render.add_node(&m_ball_trim_node);
			render.add_node(&m_base_node);
			render.add_node(&m_cyc_node);

			// Add light nodes
			render.add_node(&m_spot_light_key_node);
			render.add_node(&m_spot_light_fill_node);
			render.add_node(&m_point_light_fill_node);
			render.add_node(&m_point_light_fill_2_node);
			render.add_node(&m_spot_light_bg_01_node);
			render.add_node(&m_spot_light_bg_02_node);
		}

		void demo_01()
		{


			glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };

			add_member_nodes_to_renderer(render);

			glen::Timer timer;
			while (render.poll_events())
			{
				render.update(&m_window, &timer);
			}
		}

		void demo_02()
		{


			glen::BlinnMaterial gloss_checker_material{ "Gloss Checker" };
			gloss_checker_material.set_sampler_value(gloss_checker_material.k_material_diffuse, 0.25f);
			gloss_checker_material.set_sampler_value(gloss_checker_material.k_material_specular, 1.0f);
			gloss_checker_material.set_uniform(gloss_checker_material.k_material_specular_amount, 1.0f);
			m_checker_texture.set_min_filter(GL_NEAREST);
			m_checker_texture.set_mag_filter(GL_NEAREST);
			m_checker_texture.process();
			gloss_checker_material.set_texture(gloss_checker_material.k_material_glossiness, &m_checker_texture);

			m_base_node.set_material(&gloss_checker_material);
			m_ball_node.set_material(&gloss_checker_material);


			glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };
			add_member_nodes_to_renderer(render);

			glen::Timer timer;
			while (render.poll_events())
			{
				render.update(&m_window, &timer);
			}
		}

	private:
		glen::Window m_window{ "GL_Engine", k_width, k_height };

		glen::TargetCamera m_target_cam{};
		glen::CameraNode m_target_cam_node{ "Target Camera 1", &m_target_cam };

		glen::BlinnMaterial m_white_blinn_material{ "Blinn Material" };
		glen::BlinnMaterial m_white_diffuse_material{ "White Diffuse Material" };
		glen::BlinnMaterial m_rubber_material{ "Rubber Material" };
		glen::BlinnMaterial m_grid_material{ "Grid Material" };
		glen::Texture m_grid_texture{ "greyGrid_01.tga" };
		glen::Texture m_checker_texture{ "checker_02.tga" };

		glen::Mesh m_ball_mesh{ glen::OBJ_Loader::load_obj("shaderball_hd_ball.obj") };
		glen::Mesh m_ball_inside_mesh{ glen::OBJ_Loader::load_obj("shaderball_hd_ball_inside.obj") };
		glen::Mesh m_ball_trim_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_ball_trim.obj");
		glen::Mesh m_base_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_base.obj");
		glen::Mesh m_cyc_mesh = glen::OBJ_Loader::load_obj("shaderball_hd_cyc.obj");

		glen::MeshNode m_ball_node{ "Ball Node", &m_ball_mesh, &m_white_blinn_material };
		glen::MeshNode m_ball_inside_node{ "Ball Inside Node", &m_ball_inside_mesh, &m_white_diffuse_material };
		glen::MeshNode m_ball_trim_node{ "Ball Trim Node", &m_ball_trim_mesh, &m_rubber_material };
		glen::MeshNode m_base_node{ "Base Node", &m_base_mesh, &m_white_blinn_material };
		glen::MeshNode m_cyc_node{ "Cyc Node", &m_cyc_mesh, &m_grid_material };

		glen::SpotLight m_spot_light_key{ 140.0f, {1.0f, 1.0f, 1.0f} };
		glen::LightNode m_spot_light_key_node{ "Spot Light Key 1", &m_spot_light_key };
		glen::ShadowMap m_spot_light_key_shadow{ &m_spot_light_key_node, 4096 };

		glen::SpotLight m_spot_light_fill{ 0.0f, {0.0f, 0.0f, 0.0f} };
		glen::LightNode m_spot_light_fill_node{ "Spot Light Fill 1", &m_spot_light_fill };
		glen::ShadowMap m_spot_light_fill_shadow{ &m_spot_light_fill_node, 4096 };

		glen::PointLight m_point_light_fill{ 0.0, {0.0f, 0.0f, 0.0f} };
		glen::LightNode m_point_light_fill_node{ "Point Light Fill 1", &m_point_light_fill };

		glen::PointLight m_point_light_fill_2{ 0.0, {0.0f, 0.0f, 0.0f} };
		glen::LightNode m_point_light_fill_2_node{ "Point Light Fill 2", &m_point_light_fill_2 };

		glen::SpotLight m_spot_light_bg_01{ 140.0f, {1.0f, 1.0f, 1.0f} };
		glen::LightNode m_spot_light_bg_01_node{ "Spot Light BG 1", &m_spot_light_bg_01 };

		glen::SpotLight m_spot_light_bg_02{ 140.0f, {1.0f, 1.0f, 1.0f} };
		glen::LightNode m_spot_light_bg_02_node{ "Spot Light BG 2", &m_spot_light_bg_02 };

	};
}
#endif
