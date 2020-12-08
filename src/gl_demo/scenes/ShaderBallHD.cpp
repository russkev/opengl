#include "ShaderBallHD.h"

namespace gl_demo
{
	ShaderBallHD::ShaderBallHD()
	{
		glm::vec3 centre_focus{ 0.0f, 110.0f, 0.0f };
		m_target_cam.set_position({ 0.0f, 320.0f, 600.0f });
		m_target_cam.set_focus_target(centre_focus);
		//m_target_cam.focus(centre_focus);
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

		m_spot_light_rim.set_brightness(300.0f);
		m_spot_light_rim.set_color(glm::vec3(0.8f, 0.85f, 1.0f));
		m_spot_light_rim.set_inner_angle(20.0f);
		m_spot_light_rim.set_outer_angle(30.0f);

		m_spot_light_rim_node.set_rotation({ 40.0f, 0.0f, 0.0f });
		m_spot_light_rim_node.set_position({ 550.0f, 400.0f, -550.0f });

		m_spot_light_rim_shadow.set_bias(0.00006f);
		m_spot_light_rim_shadow.set_radius(7.0f);
		m_spot_light_rim_shadow.set_num_samples(5);
		m_spot_light_rim_shadow.set_clip_far(4000);

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

	void ShaderBallHD::reset_materials()
	{
		m_ball_node.set_material(&m_white_blinn_material);
		m_ball_inside_node.set_material(&m_white_diffuse_material);
		m_ball_trim_node.set_material(&m_rubber_material);
		m_base_node.set_material(&m_white_blinn_material);
		m_cyc_node.set_material(&m_grid_material);
	}

	void ShaderBallHD::add_member_nodes_to_renderer(glen::Renderer& render)
	{
		set_render_settings(render);
		add_lights_to_renderer(render);
		add_geometry_to_renderer(render);
	}

	void ShaderBallHD::set_render_settings(glen::Renderer& render)
	{
		render.disable_post_effects();
		render.disable_deferred_render();
		render.enable_ao();
	}

	void ShaderBallHD::add_lights_to_renderer(glen::Renderer& render)
	{
		render.add_node(&m_spot_light_key_node);
		render.add_node(&m_spot_light_fill_node);
		render.add_node(&m_spot_light_rim_node);
		render.add_node(&m_point_light_fill_node);
		render.add_node(&m_point_light_fill_2_node);
		render.add_node(&m_spot_light_bg_01_node);
		render.add_node(&m_spot_light_bg_02_node);
	}

	void ShaderBallHD::add_geometry_to_renderer(glen::Renderer& render)
	{
		render.add_node(&m_ball_node);
		render.add_node(&m_ball_inside_node);
		render.add_node(&m_ball_trim_node);
		render.add_node(&m_base_node);
		render.add_node(&m_cyc_node);
	}

	void ShaderBallHD::demo_01()
	{


		glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };

		add_member_nodes_to_renderer(render);

		while (render.poll_events())
		{
			m_spot_light_rim.set_brightness(300.0f);
			m_spot_light_rim.set_color(glm::vec3(0.8f, 0.85f, 1.0f));
			m_spot_light_rim.set_inner_angle(10.0f);
			m_spot_light_rim.set_outer_angle(20.0f);

			m_spot_light_rim_node.set_rotation({ 10.0f, 30.0f, 0.0f });
			m_spot_light_rim_node.set_position({ -350.0f, 300.0f, -650.0f });

			m_spot_light_rim_shadow.set_bias(0.000006f);
			m_spot_light_rim_shadow.set_radius(5.0f);
			m_spot_light_rim_shadow.set_num_samples(5);
			m_spot_light_rim_shadow.set_clip_far(4000);

			render.update(&m_window, &m_timer);
		}
	}

	void ShaderBallHD::demo_02()
	{
		reset_materials();

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

		while (render.poll_events())
		{
			render.update(&m_window, &m_timer);
		}
	}

	void ShaderBallHD::demo_03()
	{
		reset_materials();

		glen::BlinnMaterial dirt_material{ "Dirt Material" };
		glen::Texture dirt_diffuse{ "textures/dirt/dirt_08_diffuse.tga", true };
		glen::Texture dirt_height{ "textures/dirt/dirt_08_height.tga" };
		glen::Texture dirt_normal{ "textures/dirt/dirt_08_normal.tga" };
		glen::Texture dirt_glossiness{ "textures/dirt/dirt_08_glossiness.tga" };

		dirt_material.set_texture(dirt_material.k_material_diffuse, &dirt_diffuse);

		dirt_material.set_texture(dirt_material.k_material_normal, &dirt_normal);
		dirt_material.set_uniform(dirt_material.k_material_normal_directx_mode, false);

		dirt_material.set_sampler_value(dirt_material.k_material_specular, 1.0f);
		dirt_material.set_uniform(dirt_material.k_material_specular_amount, 1.0f);
		dirt_material.set_texture(dirt_material.k_material_glossiness, &dirt_glossiness);

		dirt_material.set_uniform(dirt_material.k_material_displacement_enabled, true);
		dirt_material.set_uniform(dirt_material.k_material_displacement_amount, 0.001f);
		dirt_material.set_texture(dirt_material.k_material_displacement, &dirt_height);


		m_base_node.set_material(&dirt_material);
		m_ball_node.set_material(&dirt_material);



		glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };
		add_member_nodes_to_renderer(render);

		while (render.poll_events())
		{

			render.update(&m_window, &m_timer);
		}
	}

	void ShaderBallHD::demo_04()
	{
		reset_materials();

		glen::BlinnMaterial tiles_material{ "Tiles Material" };
		glen::Texture tiles_diffuse{ "textures/tiles/tiles_06_diffuse_ao.tga", true };
		glen::Texture tiles_normal{ "textures/tiles/tiles_06_normal.tga" };
		glen::Texture tiles_glossiness{ "textures/tiles/tiles_06_glossiness.tga" };

		tiles_material.set_texture(tiles_material.k_material_diffuse, &tiles_diffuse);

		tiles_material.set_texture(tiles_material.k_material_normal, &tiles_normal);
		tiles_material.set_uniform(tiles_material.k_material_normal_directx_mode, false);

		tiles_material.set_sampler_value(tiles_material.k_material_specular, 1.0f);
		tiles_material.set_uniform(tiles_material.k_material_specular_amount, 1.0f);
		tiles_material.set_texture(tiles_material.k_material_glossiness, &tiles_glossiness);


		m_base_node.set_material(&tiles_material);
		m_ball_node.set_material(&tiles_material);



		glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };
		add_member_nodes_to_renderer(render);

		while (render.poll_events())
		{

			render.update(&m_window, &m_timer);
		}
	}

	void ShaderBallHD::demo_05()
	{
		reset_materials();

		glen::BlinnMaterial bricks_material{ "Bricks Material" };
		glen::Texture bricks_diffuse{ "textures/bricks/bricks_diffuse.tga", true };
		glen::Texture bricks_normal{ "textures/bricks/bricks_normal.tga" };
		glen::Texture bricks_displacement{ "textures/bricks/bricks_displacement.tga" };

		bricks_material.set_texture(bricks_material.k_material_diffuse, &bricks_diffuse);

		bricks_material.set_texture(bricks_material.k_material_normal, &bricks_normal);
		bricks_material.set_uniform(bricks_material.k_material_normal_directx_mode, false);

		bricks_material.set_sampler_value(bricks_material.k_material_specular, 1.0f);
		bricks_material.set_uniform(bricks_material.k_material_specular_amount, 1.0f);
		bricks_material.set_sampler_value(bricks_material.k_material_glossiness, 0.4f);

		bricks_material.set_uniform(bricks_material.k_material_displacement_enabled, true);
		bricks_material.set_uniform(bricks_material.k_material_displacement_amount, 0.1f);
		bricks_material.set_texture(bricks_material.k_material_displacement, &bricks_displacement);


		m_ball_inside_node.set_material(&bricks_material);

		glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };
		add_member_nodes_to_renderer(render);

		while (render.poll_events())
		{
			render.update(&m_window, &m_timer);
		}
	}

	void ShaderBallHD::demo_06()
	{
		reset_materials();

		glen::BlinnMaterial leather_material{ "Leather Material" };
		glen::Texture leather_diffuse{ "textures/leather/leather_09_diffuse.tga", true };
		glen::Texture leather_normal{ "textures/leather/leather_09_normal.tga" };
		glen::Texture leather_glossiness{ "textures/leather/leather_09_glossiness.tga" };

		leather_material.set_texture(leather_material.k_material_diffuse, &leather_diffuse);

		leather_material.set_texture(leather_material.k_material_normal, &leather_normal);
		leather_material.set_uniform(leather_material.k_material_normal_directx_mode, false);

		leather_material.set_sampler_value(leather_material.k_material_specular, 1.0f);
		leather_material.set_uniform(leather_material.k_material_specular_amount, 1.0f);
		leather_material.set_texture(leather_material.k_material_glossiness, &leather_glossiness);

		m_base_node.set_material(&leather_material);
		m_ball_node.set_material(&leather_material);

		glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };
		add_member_nodes_to_renderer(render);

		while (render.poll_events())
		{
			render.update(&m_window, &m_timer);
		}
	}

	void ShaderBallHD::demo_07()
	{
		reset_materials();

		glen::PointLight point_light{};
		point_light.set_brightness(60.0f);
		point_light.set_color({ 0.05f, 0.7f, 1.0f });
		point_light.enable_mesh();

		glen::LightNode point_light_node{ "Point Light Node", &point_light };
		point_light_node.set_position({ 90.0f, 90.0f, 100.0f });
		point_light_node.set_scale(4.0f);

		glen::ShadowMap point_light_shadow{ &point_light_node, 1024 };
		point_light_shadow.set_bias(0.0006f);
		point_light_shadow.set_radius(2.0f);
		point_light_shadow.set_num_samples(5);
		point_light_shadow.set_clip_far(4000);


		glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };
		render.enable_post_effects();
		render.disable_ao();
		render.disable_deferred_render();

		// !!! Lights still being called in from previous scene
		// !!! Possibly because destructors being called incorrectly

		render.add_node(&point_light_node);
		add_geometry_to_renderer(render);




		while (render.poll_events())
		{



			render.update(&m_window, &m_timer);
		}

	}

	void ShaderBallHD::demo_08()
	{
		reset_materials();

		glen::BlinnMaterial dirt_material{ "Dirt Material" };
		glen::Texture dirt_diffuse{ "textures/dirt/dirt_08_diffuse.tga", true };
		glen::Texture dirt_height{ "textures/dirt/dirt_08_height.tga" };
		glen::Texture dirt_normal{ "textures/dirt/dirt_08_normal.tga" };
		glen::Texture dirt_glossiness{ "textures/dirt/dirt_08_glossiness.tga" };

		dirt_material.set_texture(dirt_material.k_material_diffuse, &dirt_diffuse);

		dirt_material.set_texture(dirt_material.k_material_normal, &dirt_normal);
		dirt_material.set_uniform(dirt_material.k_material_normal_directx_mode, false);

		dirt_material.set_sampler_value(dirt_material.k_material_specular, 1.0f);
		dirt_material.set_uniform(dirt_material.k_material_specular_amount, 1.0f);
		dirt_material.set_texture(dirt_material.k_material_glossiness, &dirt_glossiness);

		dirt_material.set_uniform(dirt_material.k_material_displacement_enabled, true);
		dirt_material.set_uniform(dirt_material.k_material_displacement_amount, 0.001f);
		dirt_material.set_texture(dirt_material.k_material_displacement, &dirt_height);


		m_base_node.set_material(&dirt_material);
		m_ball_node.set_material(&dirt_material);



		glen::Renderer render{ &m_target_cam_node, glm::uvec2{ m_window.width(), m_window.height() } };
		add_member_nodes_to_renderer(render);

		while (render.poll_events())
		{

			render.update(&m_window, &m_timer);
		}
	}
}